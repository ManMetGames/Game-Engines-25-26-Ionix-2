#include "SpriteComponent.h"
#include <Graphics/QueueRenderer.h>
#include "Fysics/FysicsBody.h"

namespace IonixEngine {

	//Constructors

	SpriteComponent::SpriteComponent(Entity* entity, std::string alias, int x, int y, int zedOrder) : Component(entity, false, true, false) {
		texture = IonixEngine::TextureManager::Get().GetTexture(alias).GetTexture(); //adding sprite image file to the texture manager
		zOrder = zedOrder;
		isReversing = false;
		playbackMode = playbackOptions::FORWARD;
		boxColliderSize = b2Vec2{ 1 + (0.02f * (width - 75)), 1 + (0.02f * (height - 75)) };
		rows = 1; //default spritesheet size, can be changed in appropriate setters
		cols = 1;

		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);

		// Auto-size width/height from texture if 0 is passed
		width = (x == 0) ? size.x : x;
		height = (y == 0) ? size.y : y;

		// Also set sprite frame size to match (for single-image textures)
		spriteWidth = (x == 0) ? size.x : 32;
		spriteHeight = (y == 0) ? size.y : 32;

		calculateTotalFrames();
		initialiseSpritesheet();
	}

	SpriteComponent::SpriteComponent(Entity* entity, uint32_t hash, int x, int y, int zedOrder) : Component(entity, false, true, false) {
		texture = IonixEngine::TextureManager::Get().GetTexture(hash).GetTexture(); //adding sprite image file to the texture manager
		//std::cout << texture << std::endl;
		IonixEngine::TextureManager::Get().GetTexture(hash);
		zOrder = zedOrder;
		isReversing = false;
		playbackMode = playbackOptions::FORWARD;

		//setRowsAndCols(2, 8);
		cols = 1;
		rows = 1;

		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
		/*
		// Auto-size width/height from texture if 0 is passed
		width = (x == 0) ? size.x : x;
		height = (y == 0) ? size.y : y;
		*/
		width = x;
		height = y;
		// Also set sprite frame size to match (for single-image textures)
		spriteWidth = (x == 0) ? 32 : size.x;
		spriteHeight = (y == 0) ? 32 : size.y;

		calculateTotalFrames();
		initialiseSpritesheet();
	}

	// Animation setup

	SpriteComponent::SpriteComponent(Entity* entity, std::string alias, int x, int y, int zedOrder,
		int rows_, int cols_, int spriteW, int spriteH)
		: SpriteComponent(entity, alias, x, y, zedOrder) {
		setAnimation(rows_, cols_, spriteW, spriteH);
		calculateTotalFrames();
		initialiseSpritesheet();
	}

	SpriteComponent::SpriteComponent(Entity* entity, uint32_t hash, int x, int y, int zedOrder,
		int rows_, int cols_, int spriteW, int spriteH)
		: SpriteComponent(entity, hash, x, y, zedOrder) {
		setAnimation(rows_, cols_, spriteW, spriteH);
		calculateTotalFrames();
		initialiseSpritesheet();
	}

	SpriteComponent::SpriteComponent(Entity* entity, std::string alias, int x, int y, int zedOrder,
		int rows_, int cols_, int spriteW, int spriteH,
		playbackOptions mode, int endFrame_)
		: SpriteComponent(entity, alias, x, y, zedOrder) {
		setAnimation(rows_, cols_, spriteW, spriteH);
		calculateTotalFrames();
		playbackMode = mode;
		if (endFrame_ >= 0) endFrame = endFrame_;
		initialiseSpritesheet();
	}

	void SpriteComponent::Render(RenderData* data)
	{
		//if (entity->layer != camera->layer) { return; }
		// src is the indivudal frame we're rendering
		src.x = spriteWidth * currentCol;
		Vec2 position = entity->transform.GetGlobalPosition();

		//if (currentCol == cols) {
		//    currentRow++;
		//    currentCol = 0;
		//}
		//if (src.x < 0) {
		//    currentCol = cols;
		//    currentRow--;
		//}

		// Get rotation from physics body if it exists, otherwise use transform rotation
		double angleDegrees = 0.0;
		if (manualRotation >= 0.0f) {
			// Use manual override if set
			angleDegrees = manualRotation;
		}
		else {
			FysicsBody* fysicsBody = entity->GetComponent<FysicsBody>();
			if (fysicsBody) {
				// Box2D returns radians, SDL expects degrees
				float angleRadians = fysicsBody->GetAngle(entity);
				angleDegrees = angleRadians * (180.0 / 3.14159265358979323846);
			}
			else {
				// Transform rotation is in degrees
				angleDegrees = entity->transform.GetGlobalRotation();
			}
		}

		//create and send render data to the render queue
		data->queue->AddToQueue(RenderCall{
			texture,
			SDL_Rect { (int)(position.x), (int)(position.y), (int)width, (int)height },
			SDL_Rect { spriteWidth * currentCol, spriteHeight * currentRow, spriteWidth, spriteHeight },
			0,  // z-order (not being used in current RenderCall)
			angleDegrees
			});


		//This is just here so we can see the animation play at a normal speed
		//THIS WILL BE REMOVED


		//if ((currentFrame != endFrame) && playbackMode != playbackOptions::ONEFRAME)
		//{
		//	if (isReversing) {
		//		currentFrame--;
		//		currentCol--;
		//	}
		//	else {
		//		currentFrame++;
		//		currentCol++;
		//	}
		//}

		//else {
		//	switch (playbackMode) {
		//	case playbackOptions::FORWARD:
		//		currentFrame = 0;
		//		currentCol = 0;
		//		currentRow = 0;
		//		break;
		//	case playbackOptions::BACKWARD:
		//		currentFrame = totalFrames;
		//		currentCol = cols - 1;
		//		currentRow = rows - 1;
		//		break;
		//	case playbackOptions::FORWARDANDBACKWARD:
		//		if (isReversing) {
		//			isReversing = false;
		//			currentFrame = 0;
		//			endFrame = totalFrames;
		//		}
		//		else {
		//			isReversing = true;
		//			endFrame = 0;
		//			currentFrame = totalFrames;
		//		}
		//		break;
		//	case playbackOptions::PLAYONCE: case playbackOptions::ONEFRAME:
		//		break;
		//	}
		//}
	}

	void SpriteComponent::Update(float deltaTime)
	{
		timer += deltaTime;

		while (timer > tickRate) {
			timer -= tickRate;

			currentCol++;
			if (currentCol == cols) {
				currentCol = 0;
				currentRow++;
			}

			if (currentRow == rows) {
				currentCol = 0;
				currentRow = 0;
			}
		}
	}

	void SpriteComponent::calculateTotalFrames()
	{
		totalFrames = rows * cols;
		endFrame = totalFrames - 1;
	}

	void SpriteComponent::changeTexture(std::string alias)
	{
		texture = IonixEngine::TextureManager::Get().GetTexture(alias).GetTexture();
	}

	void SpriteComponent::initialiseSpritesheet()
	{
		switch (playbackMode) {
		case playbackOptions::FORWARD: case playbackOptions::FORWARDANDBACKWARD: case playbackOptions::PLAYONCE:
			endFrame = totalFrames;
			currentFrame = 0;
			currentRow = 0; //0 indexed
			currentCol = 0; //0 indexed
			break;
		case playbackOptions::BACKWARD:
			isReversing = true;
			endFrame = 0;
			currentFrame = totalFrames;
			currentCol = cols - 1;
			currentRow = rows - 1;
			break;
		case playbackOptions::ONEFRAME:
			currentFrame = 0;
			break;
		}
	}

	void SpriteComponent::setAnimation(int x, int y, int spriteX, int spriteY)
	{
		rows = x;
		cols = y;
		spriteWidth = spriteX;
		spriteHeight = spriteY;
	}

	//setters
	void SpriteComponent::setEndFrame(int x) { endFrame = x; }
	void SpriteComponent::setPlaybackMode(enum playbackOptions x) { playbackMode = x; }
	void SpriteComponent::setCurrentFrame(int x) { if (!(x > totalFrames)) { currentFrame = x; } }
	void SpriteComponent::setRows(int x) { rows = x; }
	void SpriteComponent::setCols(int x) { cols = x; }
	void SpriteComponent::setRowsAndCols(int Rows, int Cols)
	{
		if (Rows > Cols && Rows > 1) { rows = Rows - 1; cols = Cols; }
		else { rows = Rows; cols = Cols - 1; }
	}
	void SpriteComponent::setSpriteWidth(int x) { spriteWidth = x; }
	void SpriteComponent::setSpriteHeight(int x) { spriteHeight = x; }
	void SpriteComponent::setZedOrder(int x) { zOrder = x; }
	void SpriteComponent::setWidth(int x) { width = x; }
	void SpriteComponent::setHeight(int x) { height = x; }
	void SpriteComponent::setTickRate(float x) { tickRate = x; }

	void SpriteComponent::setBoxColliderSize(b2Vec2 newSize) { boxColliderSize = newSize; }
	void SpriteComponent::setRotation(float degrees) {
		manualRotation = degrees;
	}

	//getters
	IonixEngine::playbackOptions SpriteComponent::getPlaybackMode() /*oh lawd he big*/ { return playbackOptions(); }
	int SpriteComponent::getCurrentFrame() { return currentFrame; }
	int SpriteComponent::getEndFrame() { return endFrame; }
	int SpriteComponent::getRows() { return rows; }
	int SpriteComponent::getCols() { return cols; }
	int SpriteComponent::getSpriteWidth() { return spriteWidth; }
	int SpriteComponent::getSpriteHeight() { return spriteHeight; }
	int SpriteComponent::getZedOrder() { return zOrder; }
	int SpriteComponent::getTotalFrames() { return totalFrames; }
	int SpriteComponent::getCurrentCol() { return currentCol; }
	int SpriteComponent::getCurrentRow() { return currentRow; }
	int SpriteComponent::getWidth() { return width; }
	int SpriteComponent::getHeight() { return height; }
	int SpriteComponent::gettickRate() { return tickRate; }
	b2Vec2 SpriteComponent::getBoxColliderSize() { return boxColliderSize; }
	float SpriteComponent::getRotation() {
		// If manual rotation is set, return it; otherwise fall back to transform rotation
		return (manualRotation >= 0.0f) ? manualRotation : entity->transform.GetGlobalRotation();
}
