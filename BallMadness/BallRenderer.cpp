#include "BallRenderer.h"


void BallRenderer::renderBalls(Inferno::SpriteBatch& spriteBatch,
							   std::vector<Ball>& balls, const glm::mat4& pMatrix) {
	//Lazy init program
	if (m_program == nullptr) {
		m_program = std::make_unique<Inferno::GLSLProgram>();
		m_program->compileShaders("Shaders/defaultVert.glsl", "Shaders/defaultFrag.glsl");
		m_program->addAttribure("vertexPosition");
		m_program->addAttribure("vertexColor");
		m_program->addAttribure("vertexUV");
		m_program->linkShaders();
	}

	m_program->use();

	spriteBatch.begin();
	GLint textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	
	// Grab the camera matrix
	glm::mat4 projectionMatrix = pMatrix;
	GLint pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	for (auto& ball : balls) {
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
								ball.radius * 2.0f, ball.radius * 2.0f);

		spriteBatch.draw(destRect, uvRect, ball.textureId, 0.0f, ball.color);
	}

	spriteBatch.end();
	spriteBatch.renderBatch();

	m_program->unuse();
}

void MomentumBallRenderer::renderBalls(Inferno::SpriteBatch& spriteBatch, std::vector<Ball>& balls, const glm::mat4& pMatrix)
{
	if (m_program == nullptr) {
		m_program = std::make_unique<Inferno::GLSLProgram>();
		m_program->compileShaders("Shaders/defaultVert.glsl", "Shaders/defaultFrag.glsl");
		m_program->addAttribure("vertexPosition");
		m_program->addAttribure("vertexColor");
		m_program->addAttribure("vertexUV");
		m_program->linkShaders();
	}

	m_program->use();

	spriteBatch.begin();
	GLint textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = pMatrix;
	GLint pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	for (auto& ball : balls) {
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
			ball.radius * 2.0f, ball.radius * 2.0f);

		Inferno::ColorRGB8 color;
		GLubyte colorVal = (GLubyte)(glm::clamp(glm::length(ball.velocity) * ball.mass * 12.0f, 0.0f, 255.0f));
		color.r = colorVal;
		color.g = colorVal;
		color.b = colorVal;
		color.a = colorVal;
		spriteBatch.draw(destRect, uvRect, ball.textureId, 0.0f, color);
	}

	spriteBatch.end();
	spriteBatch.renderBatch();

	m_program->unuse();
}

VelocityBallRenderer::VelocityBallRenderer(int screenWidth, int screenHeight): m_screenWidth(screenWidth), m_screenHeight(screenHeight)
{
}

void VelocityBallRenderer::renderBalls(Inferno::SpriteBatch& spriteBatch, std::vector<Ball>& balls, const glm::mat4& pMatrix)
{
	if (m_program == nullptr) {
		m_program = std::make_unique<Inferno::GLSLProgram>();
		m_program->compileShaders("Shaders/defaultVert.glsl", "Shaders/defaultFrag.glsl");
		m_program->addAttribure("vertexPosition");
		m_program->addAttribure("vertexColor");
		m_program->addAttribure("vertexUV");
		m_program->linkShaders();
	}

	m_program->use();

	spriteBatch.begin();
	GLint textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = pMatrix;
	GLint pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	for (auto& ball : balls) {
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
			ball.radius * 2.0f, ball.radius * 2.0f);

		Inferno::ColorRGB8 color;

		float mult = 100.0f;
		GLubyte colorVal = (GLubyte)(glm::clamp(ball.velocity.x * mult, 0.0f, 255.0f));
		color.r = 128;
		color.g = (GLubyte)((ball.position.x / m_screenWidth) * 255.0f);
		color.b = (GLubyte)((ball.position.y / m_screenHeight) * 255.0f);
		color.a = colorVal;
		spriteBatch.draw(destRect, uvRect, ball.textureId, 0.0f, color);
	}

	spriteBatch.end();
	spriteBatch.renderBatch();

	m_program->unuse();
}

void ShadowBallRenderer::renderBalls(Inferno::SpriteBatch& spriteBatch, std::vector<Ball>& balls, const glm::mat4& pMatrix) {

	if (m_program == nullptr) {
		m_program = std::make_unique<Inferno::GLSLProgram>();
		m_program->compileShaders("Shaders/defaultVert.glsl", "Shaders/defaultFrag.glsl");
		m_program->addAttribure("vertexPosition");
		m_program->addAttribure("vertexColor");
		m_program->addAttribure("vertexUV");
		m_program->linkShaders();
	}

	m_program->use();

	spriteBatch.begin();

	GLint textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = pMatrix;
	GLint pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	for (auto& ball : balls) {
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

		// Calculate shadow offset and scale based on ball's velocity
		float speed = glm::length(ball.velocity);
		float shadowOffset = 5.0f * speed;
		float shadowScale = 1.0f - 0.2f * speed; // Shadow becomes smaller when speed increases, indicating height.

		const glm::vec4 shadowRect(ball.position.x - ball.radius * shadowScale + shadowOffset,
			ball.position.y - ball.radius * shadowScale - shadowOffset,
			ball.radius * 2.0f * shadowScale, ball.radius * 2.0f * shadowScale);

		// Render shadow with a dark and transparent color
		Inferno::ColorRGB8 shadowColor;
		shadowColor.r = 50;
		shadowColor.g = 50;
		shadowColor.b = 50;
		shadowColor.a = 100; // semi-transparent
		spriteBatch.draw(shadowRect, uvRect, ball.textureId, 0.0f, shadowColor);

		// Render ball normally
		const glm::vec4 ballRect(ball.position.x - ball.radius,
			ball.position.y - ball.radius,
			ball.radius * 2.0f, ball.radius * 2.0f);

		Inferno::ColorRGB8 ballColor;
		ballColor.r = 128;
		ballColor.g = (GLubyte)((ball.position.x / m_screenWidth) * 255.0f);
		ballColor.b = (GLubyte)((ball.position.y / m_screenHeight) * 255.0f);
		ballColor.a = 255; // fully opaque
		spriteBatch.draw(ballRect, uvRect, ball.textureId, 0.0f, ballColor);
	}

	spriteBatch.end();
	spriteBatch.renderBatch();

	m_program->unuse();
}

const float TrailBallRenderer::TRAIL_SEGMENT_LIFETIME = 0.5f; // 0.5 seconds for trail fading


void TrailBallRenderer::renderBalls(Inferno::SpriteBatch& spriteBatch, std::vector<Ball>& balls, const glm::mat4& pMatrix)
{
	if (m_program == nullptr) {
		m_program = std::make_unique<Inferno::GLSLProgram>();
		m_program->compileShaders("Shaders/defaultVert.glsl", "Shaders/defaultFrag.glsl");
		m_program->addAttribure("vertexPosition");
		m_program->addAttribure("vertexColor");
		m_program->addAttribure("vertexUV");
		m_program->linkShaders();
	}

	m_program->use();

	spriteBatch.begin();

	GLint textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = pMatrix;
	GLint pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	for (size_t ballIndex = 0; ballIndex < balls.size(); ballIndex++) {
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		Ball& ball = balls[ballIndex];
		// Update or create the trail for this ball
		ballTrails[ballIndex].push_front({ ball.position, 0.0f });
		if (ballTrails[ballIndex].size() > MAX_TRAIL_LENGTH) {
			ballTrails[ballIndex].pop_back();
		}

		// Render the trail
		Inferno::ColorRGB8 trailColor = ball.color; // Assuming ball has a 'color' member
		for (auto& segment : ballTrails[ballIndex]) {
			segment.age += 0.01f; // Increment age for every segment. Adjust this value according to your frame rate.

			const glm::vec4 trailRect(segment.position.x - ball.radius,
				segment.position.y - ball.radius,
				ball.radius * 2.0f, ball.radius * 2.0f);

			// Fade the trail color based on age
			trailColor.a = (GLubyte)(255.0f * (1.0f - segment.age / TRAIL_SEGMENT_LIFETIME));
			spriteBatch.draw(trailRect, uvRect, ball.textureId, 0.0f, trailColor);

			if (segment.age >= TRAIL_SEGMENT_LIFETIME) {
				segment = ballTrails[ballIndex].back();
				ballTrails[ballIndex].pop_back();
			}
		}

		// Render ball normally
		const glm::vec4 ballRect(ball.position.x - ball.radius,
			ball.position.y - ball.radius,
			ball.radius * 2.0f, ball.radius * 2.0f);
		spriteBatch.draw(ballRect, uvRect, ball.textureId, 0.0f, ball.color);
	}
	spriteBatch.end();
	spriteBatch.renderBatch();

	m_program->unuse();
}

void HaloBallRenderer::renderBalls(Inferno::SpriteBatch& spriteBatch, std::vector<Ball>& balls, const glm::mat4& pMatrix)
{
	if (m_program == nullptr) {
		m_program = std::make_unique<Inferno::GLSLProgram>();
		m_program->compileShaders("Shaders/defaultVert.glsl", "Shaders/defaultFrag.glsl");
		m_program->addAttribure("vertexPosition");
		m_program->addAttribure("vertexColor");
		m_program->addAttribure("vertexUV");
		m_program->linkShaders();
	}

	m_program->use();

	spriteBatch.begin();

	GLint textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = pMatrix;
	GLint pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	for (size_t ballIndex = 0; ballIndex < balls.size(); ballIndex++) {
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		Ball& ball = balls[ballIndex];

		// Using x-coordinate as a pseudo-time for pulsation
		float pulsateFactor = 1.0f + 1.0f * sin(ball.position.x);
		float pulsateRadius = ball.radius * pulsateFactor;

		const glm::vec4 pulsateRect(ball.position.x - pulsateRadius,
			ball.position.y - pulsateRadius,
			pulsateRadius * 2.0f, pulsateRadius * 2.0f);

		spriteBatch.draw(pulsateRect, uvRect, ball.textureId, 0.0f, ball.color);
	}

	spriteBatch.end();
	spriteBatch.renderBatch();

	m_program->unuse();
}

