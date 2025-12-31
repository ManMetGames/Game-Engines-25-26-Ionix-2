#include "SpriteComponent.h"
#include <Graphics/QueueRenderer.h>
#include "Fysics/FysicsBody.h"

#include "Fysics/FysicsBody.h"

namespace IonixEngine {

	//Constructors

	SpriteComponent::SpriteComponent(Entity* entity, std::string alias, int x, int y, int zedOrder)
		: Component(entity, false, true, false)
	{
		texture = IonixEngine::TextureManager::Get().GetTexture(alias).GetTexture();
		zOrder = zedOrder;

		isReversing = false;
		playbackMode = playbackOptions::FORWARD;
		tickRate = 0.2f;

		rows = 1;
		cols = 1;

		renderLayer = entity->renderLayer;

		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);

		// Auto-size if 0 passed in
		width = (x == 0) ? (float)size.x : (float)x;
		height = (y == 0) ? (float)size.y : (float)y;

		// Frame size (for non-spritesheets)
		RecalcFrameSize();

		// IMPORTANT: init angle
		spriteAngle = 0.0f;

		// IMPORTANT: collider calc AFTER width/height set
		boxColliderSize = b2Vec2{
			1 + (0.02f * (width - 75)),
			1 + (0.02f * (height - 75))
		};

		calculateTotalFrames();
		initialiseSpritesheet();
	}


	SpriteComponent::SpriteComponent(Entity* entity, uint32_t hash, int x, int y, int zedOrder)
		: Component(entity, false, true, false)
	{
		texture = IonixEngine::TextureManager::Get().GetTexture(hash).GetTexture();
		zOrder = zedOrder;

		isReversing = false;
		playbackMode = playbackOptions::FORWARD;
		tickRate = 0.2f;

		rows = 1;
		cols = 1;

		renderLayer = entity->renderLayer;

		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);

		width = (x == 0) ? (float)size.x : (float)x;
		height = (y == 0) ? (float)size.y : (float)y;

		RecalcFrameSize();

		spriteAngle = 0.0f;

		boxColliderSize = b2Vec2{
			1 + (0.02f * (width - 75)),
			1 + (0.02f * (height - 75))
		};

		calculateTotalFrames();
		initialiseSpritesheet();
	}


	void SpriteComponent::Render(RenderData* data)
	{

		//if (entity->layer != camera->layer) { return; }
		// src is the indivudal frame we're rendering
		src.x = spriteWidth * currentCol;
		Vec2 position = entity->transform.GetGlobalPosition();

		//if (currentCol == cols) {
		//	currentRow++;
		//	currentCol = 0;
		//}
		//if (src.x < 0) {
		//	currentCol = cols;
		//	currentRow--;
		//}

		// Get rotation from physics body if it exists, otherwise use transform rotation
		double angleDegrees = 0.0;
		FysicsBody* fysicsBody = entity->GetComponent<FysicsBody>();
		if (fysicsBody) {
			// Box2D returns radians, SDL expects degrees
			float angleRadians = fysicsBody->GetAngle(entity);
			b2Vec2 pos = fysicsBody->GetPosition(entity);
			position.x = pos.x;
			position.y = pos.y;
			angleDegrees = angleRadians * (180.0 / 3.14159265358979323846);
		}
		else {
			// Transform rotation is in degrees
		}
		angleDegrees = entity->transform.GetGlobalRotation();


		//printf("Position: [ %.1f, %.1f ], rot: %.1f\n" , position.x, position.y, angleDegrees);

		//create and send render data to the render queue
		data->queue->AddToQueue(RenderCall{
			texture,
			SDL_Rect { (int)(position.x - width / 2.0f), (int)(position.y - height / 2.0f), (int)width, (int)height },
			SDL_Rect { spriteWidth * currentCol, spriteHeight * currentRow, spriteWidth, spriteHeight },
			zOrder,
			angleDegrees,
			colorR,
			colorG,
			colorB,
			static_cast<Uint8>(255),
			spriteAngle,
			renderLayer
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

	void SpriteComponent::changeTexture(std::string alias, int iRows, int iCols, int iSpriteWidth, int iSpriteHeight)
	{
		texture = IonixEngine::TextureManager::Get().GetTexture(alias).GetTexture();

		setAnimation(iRows, iCols, iSpriteWidth, iSpriteHeight);
	}

	void SpriteComponent::setTexture(uint32_t hash)
	{
		SDL_Texture* newTex = IonixEngine::TextureManager::Get().GetTexture(hash).GetTexture();
		if (!newTex) {
			std::cout << "[SpriteComponent] setTexture failed: missing texture hash " << hash << std::endl;
			return;
		}

		texture = newTex;
		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);

		// Keep current rows/cols, but refresh frame sizing from the new texture.
		RecalcFrameSize();
		calculateTotalFrames();
		initialiseSpritesheet();
	}


	void SpriteComponent::initialiseSpritesheet()
	{
		switch (playbackMode) {
			//only forward and oneframe work for now, im sorry :(
		case playbackOptions::FORWARD: case playbackOptions::FORWARDANDBACKWARD: case playbackOptions::PLAYONCE: case playbackOptions::BACKWARD:
			endFrame = totalFrames;
			currentFrame = 0;
			currentRow = 0; //0 indexed
			currentCol = 0; //0 indexed
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

	void SpriteComponent::RecalcFrameSize()
	{
		if (cols <= 0) cols = 1;
		if (rows <= 0) rows = 1;

		spriteWidth = size.x / cols;
		spriteHeight = size.y / rows;
	}

	//setters
	void SpriteComponent::setEndFrame(int x) { endFrame = x; }
	void SpriteComponent::setPlaybackMode(enum playbackOptions x) { playbackMode = x; }
	void SpriteComponent::setCurrentFrame(int x) { if (!(x > totalFrames)) { currentFrame = x; } }
	void SpriteComponent::setRows(int x)
	{
		rows = x;
		RecalcFrameSize();
		calculateTotalFrames();
		initialiseSpritesheet();
	}
	void SpriteComponent::setCols(int x)
	{
		cols = x;
		RecalcFrameSize();
		calculateTotalFrames();
		initialiseSpritesheet();
	}
	void SpriteComponent::setRowsAndCols(int Rows, int Cols)
	{
		rows = Rows;
		cols = Cols;
		RecalcFrameSize();
		calculateTotalFrames();
		initialiseSpritesheet();
	}
	void SpriteComponent::setSpriteWidth(int x) { spriteWidth = x; }
	void SpriteComponent::setSpriteHeight(int x) { spriteHeight = x; }
	void SpriteComponent::setZedOrder(int x) { zOrder = x; }
	void SpriteComponent::setWidth(int x) { width = x; }
	void SpriteComponent::setHeight(int x) { height = x; }
	void SpriteComponent::setAngle(float angle) { spriteAngle = angle; }
	void SpriteComponent::setTickRate(float x) { tickRate = x; }

	void SpriteComponent::setBoxColliderSize(b2Vec2 newSize) { boxColliderSize = newSize; }
	void SpriteComponent::setColor(Uint8 r, Uint8 g, Uint8 b) { colorR = r; colorG = g; colorB = b; }

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
	float SpriteComponent::getAngle() { return spriteAngle; }
	int SpriteComponent::getTickRate() { return tickRate; }
	b2Vec2 SpriteComponent::getBoxColliderSize() { return boxColliderSize; }
}
