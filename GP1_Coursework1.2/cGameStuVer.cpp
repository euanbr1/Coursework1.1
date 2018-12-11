/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"
#include <chrono>
#include <thread>

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	/* Let the computer pick a random number */
	random_device rd;    // non-deterministic engine 
	mt19937 gen{ rd() }; // deterministic engine. For most common uses, std::mersenne_twister_engine, fast and high-quality.
	uniform_int_distribution<> EnemyDis{ -5, 5 };

	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();	

	theAreaClicked = { 0, 0 };
	// Store the textures
	textureName = { "sea", "bottle", "ship","enemy","theBackground", "Bullet", "OpeningScreen", "ClosingScreen", "HScoreScreen", "explosion"};
	texturesToUse = { "Images/Sprites/Snow.png", "Images/Sprites/pBauble.png", "Images/Sprites/Santa.png", "Images/Sprites/Enemy1.png","Images/Bkg/Bkgnd.png", "Images/Sprites/Bullet.png", "Images/Bkg/MainMenu.png", "Images/Bkg/endGame.png","Images/Bkg/Hscore.png", "Images/Sprites/explosion.png" };
	for (unsigned int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	tempTextTexture = theTextureMgr->getTexture("sea");
	aRect = { 0, 0, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	aColour = { 228, 213, 238, 255 };
	// Store the textures
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn", "hs_btn" };
	btnTexturesToUse = { "Images/Buttons/Quit.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/Menu.png", "Images/Buttons/Start.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png", "Images/Buttons/hScore.png" };
	btnPos = { { 400, 475 }, { 400, 300 }, { 400, 300 }, { 500, 900 }, { 400, 200 }, { 740, 500 }, { 400, 300 }, { 400, 500 } };
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = gameState::menu;
	theBtnType = btnTypes::exit;
	// Create textures for Game Dialogue (text)
	fontList = { "pirate", "skeleton" };
	fontsToUse = { "Fonts/Otto.ttf", "Fonts/Otto.ttf" };
	for (unsigned int fonts = 0; fonts < fontList.size(); fonts++)
	{
		if (fonts == 0)
		{
			theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 125);
		}
		else
		{
			theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 70);
		}
	}
	// Create text Textures
	gameTextNames = { "TitleTxt", "CollectTxt", "InstructTxt", "ThanksTxt", "SeeYouTxt","BottleCount","HSTable","HScore"};
	gameTextList = { "Protect Christmas", "Save the Elves with Santa's magic Candy!", "Use 'W' and 'S' to navigate the map.", "Thanks for playing!", "See you again soon!", "Collected: ", "", "High Score"};
	for (unsigned int text = 0; text < gameTextNames.size(); text++)
	{
		if (text == 0 || text == gameTextNames.size()-1)
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("pirate")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 0, 0, 0, 0 }, { 0, 255, 0, 255 }));
		}
		else
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("skeleton")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 0, 0, 0, 0 }, { 0, 255, 0, 255 }));
		}
	}
	// Load game sounds
	soundList = { "theme", "click" };
	soundTypes = { soundType::music, soundType::sfx};
	soundsToUse = { "Audio/Theme/Kevin_MacLeod_-_Winter_Reflections.wav", "Audio/SFX/ClickOn.wav"};
	for (unsigned int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("OpeningScreen"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("OpeningScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());

	theTileMap.setMapStartXY({ 150, 100 });

	bottlesCollected = 0;
	strScore = gameTextList[5];
	strScore += to_string(bottlesCollected).c_str();
	theTextureMgr->deleteTexture("BottleCount");

	numTableItems = 0;
	theHSTable.loadFromFile("Data/HighScore.dat");

	theHighScoreTable = gameTextList[6];
	
	theHSTableSize = theHSTable.getTableSize();
	highScoreTextures = { "score1","score2","score3","score4","score5","score6","score7","score8","score9","score10" };

	// Create vector array of textures

	/*for (int elf = 0; elf < 5; elf++)
	{
		thePirates.push_back(new cEnemy);
		thePirates[elf]->setMapPosition(1, 0);
		thePirates[elf]->setSpriteTranslation({ 100, -50 });
		thePirates[elf]->setTexture(theTextureMgr->getTexture("enemy"));
		thePirates[elf]->setSpriteDimensions(theTextureMgr->getTexture("enemy")->getTWidth(), theTextureMgr->getTexture("enemy")->getTHeight());
		thePirates[elf]->setEnemyVelocity(200);
		thePirates[elf]->setActive(true);
	}*/
	for (int item = 0; item < theHSTableSize; item++)
	{
		string entry = "";
		entry += theHSTable.getItem(item).Name + " " + to_string(theHSTable.getItem(item).score);
		theTextureMgr->addTexture(highScoreTextures[item], theFontMgr->getFont("skeleton")->createTextTexture(theRenderer, entry.c_str(), textType::solid, { 0, 0, 0, 0 }, { 0, 255, 0, 255 }));
 	}

	timer = 0;
	gameOver = false;

}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case gameState::menu:
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("OpeningScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("OpeningScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 8, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("CollectTxt");
		pos = { 325, 125, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("InstructTxt");
		pos = { 325, 175, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render Button
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 500, 275});
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("hs_btn")->setSpritePos({ 460, 375 });
		theButtonMgr->getBtn("hs_btn")->render(theRenderer, &theButtonMgr->getBtn("hs_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("hs_btn")->getSpritePos(), theButtonMgr->getBtn("hs_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 475 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	break;
	case gameState::playing:
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("OpeningScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("OpeningScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 2, 2, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		theTextureMgr->addTexture("BottleCount", theFontMgr->getFont("pirate")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 0, 0, 0, 0 }, { 0, 255, 0, 255 }));
		tempTextTexture = theTextureMgr->getTexture("BottleCount");
		pos = { 600, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		theTileMap.render(theSDLWND, theRenderer, theTextureMgr, textureName);
		theTileMap.renderGridLines(theRenderer, aRect, aColour);
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 750, 600 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		
		// Render each asteroid in the vector array
		/*for (int draw = 0; draw < (int)thePirates.size(); draw++)
		{
			thePirates[draw]->render(theRenderer, &thePirates[draw]->getSpriteDimensions(), &thePirates[draw]->getMapPosition(), thePirates[draw]->getSpriteRotAngle(), &thePirates[draw]->getSpriteCentre(), thePirates[draw]->getSpriteScale());
		}
		*/
		// Render each bullet in the vector array
		for (int draw = 0; draw < (int)theBullets.size(); draw++)
		{
			theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
		}
	}
	break;
	case gameState::end:
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("ClosingScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("ClosingScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("ThanksTxt");
		pos = { 50, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("SeeYouTxt");
		pos = { 50, 175, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture = theTextureMgr->getTexture("BottleCount");
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 500, 225 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 350});
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		
		//render Score
		cTexture* tempTextTexture = theTextureMgr->getTexture("BottleCount");
		SDL_Rect pos = { 50, 175, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	}
	break;
	case gameState::highscore:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("HScore");
		pos = { 220, 125, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		pos = { 220, 200, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };

		for (int item = 0; item < theHSTableSize; item++)
		{
			tempTextTexture = theTextureMgr->getTexture(highScoreTextures[item]);
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			pos = { 220, 200 + (50 * item), tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		}

		// Render Button
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 700, 350 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 700, 450 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
	}
	break;
	case gameState::quit:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	timer += deltaTime;
	// CHeck Button clicked and change state
	if (theGameState == gameState::menu || theGameState == gameState::end)
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, gameState::quit, theAreaClicked);
	}
	else
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, gameState::end, theAreaClicked);
	}
	if (theGameState == gameState::highscore)
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("HScoreScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("HScoreScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked);
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);
	}

	if (theGameState == gameState::menu)
	{
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked);
		gameOver = false;
		if (theGameState == gameState::playing && gameOver == false)
		{

			theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
			theTileMap.update(theBottle.getMapPosition(), 1, 0.0f);
			theTileMap.update(thePirate.getMapPosition(), 1, 0.0f);
			theShip.setMapPosition(5, 8);
			theBottle.setMapPosition(4, 0);
			//thePirate.setMapPosition(1, 0);
			thePirates.push_back(new cEnemy);
			int numPirates = thePirates.size() - 1;
			thePirates[numPirates]->setMapPosition(1, 0);
			SDL_Point enemyPos = { (thePirates[numPirates]->getMapPosition().C * 64) + 150, (thePirates[numPirates]->getMapPosition().R * 64) + 100 };
			thePirates[numPirates]->setSpritePos({ enemyPos.x, enemyPos.y });
			thePirates[numPirates]->setSpriteTranslation({ 50, 1 });
			thePirates[numPirates]->setTexture(theTextureMgr->getTexture("enemy"));
			thePirates[numPirates]->setSpriteDimensions(theTextureMgr->getTexture("enemy")->getTWidth(), theTextureMgr->getTexture("enemy")->getTHeight());
			thePirates[numPirates]->setActive(true);
			cout << "Bullet added to Vector at position - x: " << theShip.getBoundingRect().x << " y: " << theShip.getBoundingRect().y << endl;

			// Lab Code goes here
			theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
			theTileMap.update(theBottle.getMapPosition(), 2, theBottle.getBottleRotation());
			theTileMap.update(thePirate.getMapPosition(), 4, thePirate.getEnemyRotation());
			
			// Lab Code goes here
			bottlesCollected = 0;

		}
		
	}

	theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);
	theGameState = theButtonMgr->getBtn("hs_btn")->update(theGameState, gameState::highscore, theAreaClicked);

	if (theGameState == gameState::playing)
	{
		
		vector<cEnemy*>::iterator ElfIterator = thePirates.begin();
		while (ElfIterator != thePirates.end())
		{
			if ((*ElfIterator)->isActive() == false)
			{
				ElfIterator = thePirates.erase(ElfIterator);
			}
			else
			{
				SDL_Point elfPos = { ((*ElfIterator)->getMapPosition().C * 64) + 150, ((*ElfIterator)->getMapPosition().R * 64) + 100 };
				(*ElfIterator)->setBoundingRect({ elfPos.x, elfPos.y });

				(*ElfIterator)->update(deltaTime);
				++ElfIterator;
				//if ((*ElfIterator)->getSpritePos().x >= WINDOW_WIDTH)
				//{
				//	(*ElfIterator)->setSpriteTranslation({ (*ElfIterator)->getSpriteTranslation().x * (-1), (*ElfIterator)->getSpriteTranslation().y * (-1) });
				//}			++ElfIterator;

			}
		}
		// Update the visibility and position of each bullet
		vector<cBullet*>::iterator bulletIterartor = theBullets.begin();
		while (bulletIterartor != theBullets.end())
		{
			if ((*bulletIterartor)->isActive() == false)
			{
				bulletIterartor = theBullets.erase(bulletIterartor);
			}
			else
			{
				(*bulletIterartor)->update(deltaTime);
				++bulletIterartor;
			}
		}
		// Update the visibility and position of each explosion
		vector<cSprite*>::iterator expIterartor = theExplosions.begin();
		while (expIterartor != theExplosions.end())
		{
			if ((*expIterartor)->isActive() == false)
			{
				expIterartor = theExplosions.erase(expIterartor);
			}
			else
			{
				(*expIterartor)->animate(deltaTime);
				++expIterartor;
			}
		}

		/*
		==============================================================
		| Check for collisions
		==============================================================
		*/
		for (vector<cBullet*>::iterator ElfIterator = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
		{
			//(*bulletIterartor)->update(deltaTime);
			for (vector<cEnemy*>::iterator ElfIterator = thePirates.begin(); ElfIterator != thePirates.end(); ++ElfIterator)
			{
				if ((*ElfIterator)->collidedWith(&(*ElfIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
				{
					// if a collision set the bullet and elf to false
					(*ElfIterator)->setActive(false);
					(*bulletIterartor)->setActive(false);
					theExplosions.push_back(new cSprite);
					int index = theExplosions.size() - 1;
					theExplosions[index]->setSpriteTranslation({ 0, 0 });
					theExplosions[index]->setActive(true);
					theExplosions[index]->setNoFrames(16);
					theExplosions[index]->setTexture(theTextureMgr->getTexture("explosion"));
					theExplosions[index]->setSpriteDimensions(theTextureMgr->getTexture("explosion")->getTWidth() / theExplosions[index]->getNoFrames(), theTextureMgr->getTexture("explosion")->getTHeight());
					theExplosions[index]->setSpritePos({ (*ElfIterator)->getSpritePos().x + (int)((*ElfIterator)->getSpritePos().w / 2), (*ElfIterator)->getSpritePos().y + (int)((*ElfIterator)->getSpritePos().h / 2) });

					theSoundMgr->getSnd("explosion")->play(0);

					// Lab 7 code goes here

				}
			}
		}

		//if ((int)timer % 4 == 0)
		//{
		//	thePirate.genRandomPos(theShip.getMapPosition(), theBottle.getMapPosition());
		//	theTileMap.update(thePirate.getMapPosition(), 4, thePirate.getEnemyRotation());
		//}
		if (theGameState == gameState::playing && thePirate.getMapPosition().R < 8, thePirate.getMapPosition().C <8) //Restrict enemy movement
		{
			thePirate.setEnemyRotation(0.0f);
			if (frames % 25 == 0)
			{
				theTileMap.update(thePirate.getMapPosition(), 1, 0.0f);
				thePirate.update(thePirate.getMapPosition().C + 1, thePirate.getMapPosition().R);
				theTileMap.update(thePirate.getMapPosition(), 4, thePirate.getEnemyRotation());


			}
			//Sleep(900);
		}

		// Check if ship has collided with the bottle
		if (theShip.getMapPosition() == theBottle.getMapPosition())
		{
			bottlesCollected++;
			theBottle.genRandomPos(theShip.getMapPosition().R, theShip.getMapPosition().C);
			theTileMap.update(theBottle.getMapPosition(), 2, theBottle.getBottleRotation());
			//Create Updated bottle count
			strScore = gameTextList[5];
			strScore += to_string(bottlesCollected).c_str();
			theTextureMgr->deleteTexture("BottleCount");

		}
		if (theGameState == gameState::playing && theBottle.getMapPosition().R < 8, theBottle.getMapPosition().C <8) //Restrict Bottle movement
		{
			theBottle.setBottleRotation(0.0f);
			theTileMap.update(theBottle.getMapPosition(), 1, 0.0f);
			if (frames % 15 == 0)
			{
				theBottle.update(theBottle.getMapPosition().C + 1, theBottle.getMapPosition().R);
			}


		}

		theTileMap.update(theBottle.getMapPosition(), 2, theBottle.getBottleRotation());
		// Check if Pirate has collided with the ship
		// Lab Code goes here

		if (gameOver)
		{
			theGameState = gameState::end;
		}

		frames++;
	}
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					theAreaClicked = { event.motion.x, event.motion.y };
					//if (theGameState == gameState::playing)
					//{
					//	theTilePicker.update(theAreaClicked);
					//	if (theTilePicker.getTilePicked() > -1)
					//	{
					//		dragTile.setSpritePos(theAreaClicked);
					//		dragTile.setTexture(theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1]));
					//		dragTile.setSpriteDimensions(theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1])->getTWidth(), theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1])->getTHeight());
					//	}
					//}
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					//if (theGameState == gameState::playing)
					//{
					//	theAreaClicked = { event.motion.x, event.motion.y };
					//	theTileMap.update(theAreaClicked, theTilePicker.getTilePicked());
					//	theTilePicker.setTilePicked(-1);
					//}
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			{
				dragTile.setSpritePos({ event.motion.x, event.motion.y });
			}
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_s:
				{
					if (theGameState == gameState::playing && theShip.getMapPosition().R < 9)
					{
						theShip.setShipRotation(0.0f);
						theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
						theShip.update(theShip.getMapPosition().C, theShip.getMapPosition().R + 1);
						theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
						// Lab Code goes here
					}
				}
				break;

				case SDLK_w:
				{
					if (theGameState == gameState::playing && theShip.getMapPosition().R > 0)
					{
						theShip.setShipRotation(0.0f);
						theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
						theShip.update(theShip.getMapPosition().C, theShip.getMapPosition().R - 1);
						theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
						// Lab Code goes here
					}
				}
				break;
				//case SDLK_RIGHT:
				//{
				//	if (theGameState == gameState::playing && theShip.getMapPosition().C < 9)
				//	{
				//		theShip.setShipRotation(270.0f);
				//		theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
				//		theShip.update(theShip.getMapPosition().C + 1, theShip.getMapPosition().R);
				//		theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
				//		// Lab Code goes here
				//	}
				//}
				//break;

				//case SDLK_LEFT:
				//{
				//	if (theGameState == gameState::playing && theShip.getMapPosition().C > 0)
				//	{
				//		theShip.setShipRotation(90.0f);
				//		theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
				//		theShip.update(theShip.getMapPosition().C - 1, theShip.getMapPosition().R);
				//		theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
				//		// Lab Code goes here
				//	}
				//}
				break;
				case SDLK_SPACE:
				{
					//if (frames % 4 == 0)
					//{
						theBullets.push_back(new cBullet);
						int numBullets = theBullets.size() - 1;
						SDL_Point bulletPos = { (theShip.getMapPosition().C * 64) + 150, (theShip.getMapPosition().R * 64) + 100 };
						theBullets[numBullets]->setSpritePos({ bulletPos.x, bulletPos.y });
						theBullets[numBullets]->setSpriteTranslation({ 50, 1 });
						theBullets[numBullets]->setTexture(theTextureMgr->getTexture("Bullet"));
						theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("Bullet")->getTWidth(), theTextureMgr->getTexture("Bullet")->getTHeight());
						theBullets[numBullets]->setBulletVelocity(50);
						theBullets[numBullets]->setSpriteRotAngle(theShip.getSpriteRotAngle());
						theBullets[numBullets]->setActive(true);
						cout << "Bullet added to Vector at position - x: " << theShip.getBoundingRect().x << " y: " << theShip.getBoundingRect().y << endl;

					//}
				}
				
				break;
				default:
					break;
				}

			default:
				break;
		}

	}
	return theLoop;

}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	//Quit FONT system
	TTF_Quit();

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

