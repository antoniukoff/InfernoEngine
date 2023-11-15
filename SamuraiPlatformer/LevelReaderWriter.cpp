#include "LevelReaderWriter.h"

#include <InfernoEngine/ResourceManager.h>
#include <fstream>
#include <iostream>

// When you want to make a new version, add it here
const unsigned int TEXT_VERSION_0 = 100;

// Make sure this is set to the current version
const unsigned int TEXT_VERSION = TEXT_VERSION_0;

bool LevelReaderWriter::saveAsText(const std::string& filePath, const Player& player, const std::vector<Box>& boxes, const std::vector<Light>& lights) {
    // Keep this updated with newest version
    return saveAsTextV0(filePath, player, boxes, lights);
}

bool LevelReaderWriter::saveAsTextV0(const std::string& filePath, const Player& player, const std::vector<Box>& boxes, const std::vector<Light>& lights) {
    // Open file and error check
    std::ofstream file(filePath);
    if (file.fail()) {
        perror(filePath.c_str());
        return false;
    }

    // Write version
    file << TEXT_VERSION << '\n';
    // Write player
    file << player.getPosition().x << ' ' << player.getPosition().y << ' '
         << player.getDrawDims().x << ' ' << player.getDrawDims().y << ' '
         << player.getCollisionDims().x << ' ' << player.getCollisionDims().y << ' '
         << player.getColor().r << ' ' << player.getColor().g << ' '
         << player.getColor().b << ' ' << player.getColor().a << '\n';

    // Write number of boxes
    file << boxes.size() << '\n';
    // Write all boxes
    for (auto& b : boxes) {
        file << b.getPosition().x << ' ' << b.getPosition().y << ' '
             << b.getDimensions().x << ' ' << b.getDimensions().y << ' '
             << b.getColor().r << ' ' << b.getColor().g << ' '
             << b.getColor().b << ' ' << b.getColor().a << ' '
             << b.getUvRect().x << ' ' << b.getUvRect().y << ' '
             << b.getUvRect().z << ' ' << b.getUvRect().w << ' '
             << b.getAngle() << ' ' << b.getTexture().filePath << ' '
             << b.getIsDynamic() << ' ' << b.getFixedRotation() << '\n';
    }

    // Write number of lights
    file << lights.size() << '\n';
    // Write all lights
    for (auto& l : lights) {
        file << l.position.x << ' ' << l.position.y << ' '
             << l.size << ' ' << l.color.r << ' ' << l.color.g << ' '
             << l.color.b << ' ' << l.color.a << '\n';
    }

    return true;
}

bool LevelReaderWriter::saveAsBinary(const std::string& filePath, const Player& player, const std::vector<Box>& boxes, const std::vector<Light>& lights) {
	std::ofstream file(filePath, std::ios::binary);
	if (!file.is_open()) {
		perror(filePath.c_str());
		return false;
	}

	// Write version
	unsigned int version = TEXT_VERSION;
	file.write(reinterpret_cast<const char*>(&version), sizeof(version));

	// Save player data
	glm::vec2 playerPos = player.getPosition();
	glm::vec2 playerDrawDims = player.getDrawDims();
	glm::vec2 playerCollisionDims = player.getCollisionDims();
	Vladgine::ColorRGB8 playerColor = player.getColor();

	file.write(reinterpret_cast<const char*>(&playerPos), sizeof(playerPos));
	file.write(reinterpret_cast<const char*>(&playerDrawDims), sizeof(playerDrawDims));
	file.write(reinterpret_cast<const char*>(&playerCollisionDims), sizeof(playerCollisionDims));
	file.write(reinterpret_cast<const char*>(&playerColor), sizeof(playerColor));

	// Save boxes data
	size_t numBoxes = boxes.size();
	file.write(reinterpret_cast<const char*>(&numBoxes), sizeof(numBoxes));

	for (const Box& b : boxes) {
		glm::vec2 pos = b.getPosition();
		glm::vec2 dims = b.getDimensions();
		Vladgine::ColorRGB8 color = b.getColor();
		glm::vec4 uvRect = b.getUvRect();
		float angle = b.getAngle();
		bool isDynamic = b.getIsDynamic();
		bool fixedRotation = b.getFixedRotation();
		std::string texturePath = b.getTexture().filePath;

		file.write(reinterpret_cast<const char*>(&pos), sizeof(pos));
		file.write(reinterpret_cast<const char*>(&dims), sizeof(dims));
		file.write(reinterpret_cast<const char*>(&color), sizeof(color));
		file.write(reinterpret_cast<const char*>(&uvRect), sizeof(uvRect));
		file.write(reinterpret_cast<const char*>(&angle), sizeof(angle));
		file.write(reinterpret_cast<const char*>(&isDynamic), sizeof(isDynamic));
		file.write(reinterpret_cast<const char*>(&fixedRotation), sizeof(fixedRotation));

		size_t pathLength = texturePath.size();
		file.write(reinterpret_cast<const char*>(&pathLength), sizeof(pathLength));
		file.write(texturePath.c_str(), pathLength);
	}

	// Save lights data
	size_t numLights = lights.size();
	file.write(reinterpret_cast<const char*>(&numLights), sizeof(numLights));

	for (const Light& l : lights) {
		glm::vec2 position = l.position;
		float size = l.size;
		Vladgine::ColorRGB8 color = l.color;

		file.write(reinterpret_cast<const char*>(&position), sizeof(position));
		file.write(reinterpret_cast<const char*>(&size), sizeof(size));
		file.write(reinterpret_cast<const char*>(&color), sizeof(color));
	}

	return true;
}


bool LevelReaderWriter::loadAsText(const std::string& filePath, b2World* world, Player& player, std::vector<Box>& boxes, std::vector<Light>& lights) {
    // Open file and error check
    std::ifstream file(filePath);
    if (file.fail()) {
        perror(filePath.c_str());
        return false;
    }

    // Get version
    unsigned int version;
    file >> version;

    // Parse based on version
    switch (version) {
        case TEXT_VERSION_0:
            loadAsTextV0(file, world, player, boxes, lights);
            break;
        default:
            puts("Unknown version number in level file. File may be corrupted...");
            return false;
    }

    return true;
}

bool LevelReaderWriter::loadAsBinary(const std::string& filePath, b2World* world, Player& player, std::vector<Box>& boxes, std::vector<Light>& lights) {
	std::ifstream file(filePath, std::ios::binary);
	if (!file) {
		perror(filePath.c_str());
		return false;
	}

	// Read the version number
	unsigned int version;
	file.read(reinterpret_cast<char*>(&version), sizeof(version));

	if (version != 100) {
		puts("Unknown version number in level file. File may be corrupted...");
		return false;
	}

	// Read player data
	glm::vec2 pos;
	glm::vec2 ddims;
	glm::vec2 cdims;
	Vladgine::ColorRGB8 color;
	file.read(reinterpret_cast<char*>(&pos), sizeof(pos));
	file.read(reinterpret_cast<char*>(&ddims), sizeof(ddims));
	file.read(reinterpret_cast<char*>(&cdims), sizeof(cdims));
	file.read(reinterpret_cast<char*>(&color), sizeof(color));
	player.init(world, pos, ddims, cdims, color);

	// Read number of boxes
	size_t numBoxes;
	file.read(reinterpret_cast<char*>(&numBoxes), sizeof(numBoxes));

	// Read all boxes
	for (size_t i = 0; i < numBoxes; i++) {
		Box b;
		glm::vec2 pos;
		glm::vec2 dims;
		glm::vec4 uvRect;
		Vladgine::ColorRGB8 color;
		bool fixedRotation;
		bool isDynamic;
		float angle;
		Vladgine::GLTexture texture;
		std::string texturePath;
		uint32_t texturePathLength;

		file.read(reinterpret_cast<char*>(&pos), sizeof(pos));
		file.read(reinterpret_cast<char*>(&dims), sizeof(dims));
		file.read(reinterpret_cast<char*>(&color), sizeof(color));
		file.read(reinterpret_cast<char*>(&uvRect), sizeof(uvRect));
		file.read(reinterpret_cast<char*>(&angle), sizeof(angle));
		file.read(reinterpret_cast<char*>(&isDynamic), sizeof(isDynamic));
		file.read(reinterpret_cast<char*>(&fixedRotation), sizeof(fixedRotation));
		std::cout << "Box " << i << " isDynamic: " << isDynamic << std::endl;
		file.read(reinterpret_cast<char*>(&texturePathLength), sizeof(texturePathLength));
		std::cout << "Box " << i << " texturePathLength: " << texturePathLength << std::endl;


		texturePath.resize(texturePathLength);
		file.read(&texturePath[0], texturePathLength);

		texture = Vladgine::ResourceManager::getTexture(texturePath);
		b.init(world, pos, dims, texture, color, fixedRotation, isDynamic, angle, uvRect);
		boxes.push_back(b);
	}

	// Read number of lights
	size_t numLights;
	file.read(reinterpret_cast<char*>(&numLights), sizeof(numLights));

	// Read all lights
	for (size_t i = 0; i < numLights; i++) {
		Light l;
		glm::vec2 pos;
		float size;
		Vladgine::ColorRGB8 color;

		file.read(reinterpret_cast<char*>(&pos), sizeof(pos));
		file.read(reinterpret_cast<char*>(&size), sizeof(size));
		file.read(reinterpret_cast<char*>(&color), sizeof(color));

		l.color = color;
		l.position = pos;
		l.size = size;

		lights.push_back(l);
	}

	return true;
}


bool LevelReaderWriter::loadAsTextV0(std::ifstream& file, b2World* world, Player& player, std::vector<Box>& boxes, std::vector<Light>& lights) {
    { // Read player
        glm::vec2 pos;
        glm::vec2 ddims;
        glm::vec2 cdims;
        Vladgine::ColorRGB8 color;
        file >> pos.x >> pos.y >> ddims.x >> ddims.y >> cdims.x >> cdims.y >> color.r >> color.g >> color.b >> color.a;
        player.init(world, pos, ddims, cdims, color);
    }

    { // Read boxes
        glm::vec2 pos;
        glm::vec2 dims;
        glm::vec4 uvRect;
        Vladgine::ColorRGB8 color;
        bool fixedRotation;
        bool isDynamic;
        float angle;
        Vladgine::GLTexture texture;
        std::string texturePath;
        size_t numBoxes;
        file >> numBoxes;
        for (size_t i = 0; i < numBoxes; i++) {
            file >> pos.x >> pos.y >> dims.x >> dims.y >> color.r >> color.g >> color.b >> color.a
                 >> uvRect.x >> uvRect.y >> uvRect.z >> uvRect.w
                 >> angle >> texturePath >> isDynamic >> fixedRotation;

            texture = Vladgine::ResourceManager::getTexture(texturePath);

            boxes.emplace_back();
            boxes.back().init(world, pos, dims, texture, color, fixedRotation, isDynamic, angle, uvRect);
        }
    }

    { // Read lights
        glm::vec2 pos;
        float size;
        Vladgine::ColorRGB8 color;
        size_t numLights;
        file >> numLights;
        for (size_t i = 0; i < numLights; i++) {
            file >> pos.x >> pos.y >> size >> color.r >> color.g >> color.b >> color.a;
            lights.emplace_back();
            lights.back().color = color;
            lights.back().position = pos;
            lights.back().size = size;
        }
    }

    return true;
}
