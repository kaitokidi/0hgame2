#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

bool isWhite(sf::Image& image, float px, float py){
    return image.getPixel(px, py) == sf::Color::White;
}

bool isBlue(sf::Image& image, float px, float py){
    return image.getPixel(px, py) == sf::Color::Blue;
}

float getAngle(sf::Vector2f &orig, sf::Vector2i &des) {
    return std::atan2(des.y - orig.y, des.x - orig.x)*180/(M_PI);
}

int main(){
    srand (time(NULL));

	//CONSTANT AND 'GLOBAL' VARIABLES
	int speed = 180;
	float time_to_next_sprite = 0.09;
    enum direction { right, left, up , down,  none};
    const int mx[5] = {1,   -1,    0,    0,   0};
    const int my[5] = {0,    0,   -1,    1,   0};
    
    
	//'VECTOR2's
	sf::Vector2u size(1100,700);
	sf::Vector2f spriteSize(32,48);
	sf::Vector2f viewPosition(0,0);
	sf::Vector2i spriteSource(0,down);
	sf::Vector2f originalSpriteSize(0,0);
	sf::Vector2u newsize(size.x, size.y);
	sf::Vector2f playerPosition(300,300);

	//SFML OBJECTS
	sf::View view;
	sf::Image image;
	sf::Event event;
	sf::Clock deltaClock;
	sf::Sprite player, background;
	sf::Texture pTexture, tbackground;
    std::vector < sf::Vector2f > positions;
    
    positions = std::vector < sf::Vector2f > ();
    positions.push_back(sf::Vector2f(0,0));
    positions.push_back(sf::Vector2f((rand()%200)-100,(rand()%200)-100));
    positions.push_back(sf::Vector2f((rand()%200)-100,(rand()%200)-100));
//    positions.push_back(sf::Vector2f((rand()%200)-100,(rand()%200)-100));
//     positions.push_back(sf::Vector2f(40,40));
//     positions.push_back(sf::Vector2f(100,-80));
    
	//LOAD IMAGES AND TEXTURES
	if(!image.loadFromFile("pics/ground.png")) std::cout << "ground Image Not Loaded " << std::endl;
	if(!tbackground.loadFromImage(image)) std::cout << "background texture Not Loaded " << std::endl;
	if(!pTexture.loadFromFile("pics/sprites.png")) std::cout << "personatge Not Loaded " << std::endl;
	spriteSize.x = originalSpriteSize.x = pTexture.getSize().x/8;
	spriteSize.y = originalSpriteSize.y = pTexture.getSize().y/4;

	//VARIABLES	
	direction d = none;
	double 	escalat = 1;
	bool mousePressed = false;
	bool keypressed = false, fly = false;
	float distx = spriteSize.x/3, disty = spriteSize.y/4;
	float deltatime = 0, movx = 0.0, movy = 0.0, scont = 0.0;

	//Set player's properties
	player.setTexture(pTexture);
	player.setPosition(playerPosition);
	//Set Background's properties
	background.setTexture(tbackground);

	//CREATE THE WINDOW
	sf::RenderWindow window(sf::VideoMode(size.x,size.y), "SuchPenguins", sf::Style::Resize|sf::Style::Close);

	//GAME LOOP
	while(window.isOpen()){

		//Loop for handling events
		while(window.pollEvent(event)){
			switch (event.type){
				//Close event
				case sf::Event::Closed:
					window.close();
					break;
				//KeyPressed event
				case  sf::Event::KeyPressed:
					//Close key
					if (event.key.code == sf::Keyboard::Escape) {
						window.close();
					}
					if (event.key.code == sf::Keyboard::Q) {
						window.close();
					}
					//Increasing Speed
					if (event.key.code == sf::Keyboard::S) {
						if(speed < 280){
							speed += 20;
							time_to_next_sprite -= 0.01;
						}
					}
					//Decreasing Speed
					if (event.key.code == sf::Keyboard::D) {
						if(speed > 80){
							speed -= 20;
							time_to_next_sprite += 0.01;
						}
					}
					break;
				//Resize event
				case sf::Event::Resized:
					newsize.x = event.size.width;
					newsize.y = event.size.height;
					distx = distx * newsize.x/size.x;
					disty = disty * newsize.x/size.x;
					speed = speed * newsize.x/size.x;
					escalat = escalat * newsize.x/size.x;
					playerPosition.x = playerPosition.x*newsize.x/size.x;
					playerPosition.y = playerPosition.y*newsize.x/size.x;
					//Try and catch for resizeing the window
					try {window.setSize(newsize);}
					catch(sf::Event e){std::cout << "Can't Resize!" << std::endl;}
					size = newsize;
					//Remove the events that could have been activated whith the previous window
					while(window.pollEvent(event)){}
					break;
				//Default
				default:
					//Do nothing
					break;
			}
		}	

		//Deltatime 
		deltatime = deltaClock.restart().asSeconds();
		scont += deltatime;

		//Update Direction 'd'
		d = none;
        
	        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
	            float mouse_x, mouse_y;
	            mouse_x = sf::Mouse::getPosition(window).x; 
	            mouse_y = sf::Mouse::getPosition(window).y;
	            if(mouse_x > window.getSize().x*2/3) 	d = right;
	            else if(mouse_x < window.getSize().x/3) d =  left;
	            else if(mouse_y < window.getSize().y/2) d =    up;
	            else if(mouse_y > window.getSize().y/2) d =  down;
                
                /*sf::Vector2i position = sf::Mouse::getPosition(window);
                position.y += view.getCenter().y-view.getSize().y/2;        
                sf::Vector2f centre = sf::Vector2f(playerPosition.x, playerPosition.y-spriteSize.y/2);
                float angle = getAngle(centre, position);
                 if(angle < 0) angle = 360 + angle;
                if( angle > 45 && angle <= 135) d = down;
                else if( angle > 135 && angle <= 225) d = left;
                else if( angle > 225 && angle <= 315) d = up;
                else d = right;*/
	        }
	        //By keyboard
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 	d = up;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	d = down;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	d = left;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	d = right;

		//If there is a direction
		if(d != none){
			//Set value to movement variables and update spritesource
			if(spriteSource.y == d){
				movx += mx[d]*speed*deltatime;
				movy += my[d]*speed*deltatime;
			}
			else spriteSource.y = d; //so you can turn without walking
			if (scont >= time_to_next_sprite){
				scont = 0;
				++spriteSource.x;
			}
		}

		//Calcule player's destination
		int destix = playerPosition.x+movx;
        int destiy = playerPosition.y+movy;
        
		//Calcule the point in the image corresponding to the point the player is in the background
		float px, py;
        px = image.getSize().x*destix/tbackground.getSize().x/background.getScale().x;
        py = image.getSize().y*destiy/tbackground.getSize().y/background.getScale().y;
		
        //Check if the color of the corresponding point is White (if it is let the player move)
        bool allinWhite = true;
        int reached = 0;
        if(!fly){
        for(int i = 0; i < positions.size(); ++i){
            
            destix += positions[i].x;
            destiy += positions[i].y;
            px += positions[i].x;
            py += positions[i].y;
            
            allinWhite *= isWhite(image, px, py) && isWhite(image, px, image.getSize().y*(destiy-disty)/tbackground.getSize().y/background.getScale().y) &&
            isWhite(image, image.getSize().x*(destix+distx)/tbackground.getSize().x/background.getScale().x, image.getSize().y*(destiy-disty)/tbackground.getSize().y/background.getScale().y) &&
            isWhite(image, image.getSize().x*(destix-distx)/tbackground.getSize().x/background.getScale().x, image.getSize().y*(destiy-disty)/tbackground.getSize().y/background.getScale().y) &&
            isWhite(image, image.getSize().x*(destix+distx)/tbackground.getSize().x/background.getScale().x, py) &&
            isWhite(image, image.getSize().x*(destix-distx)/tbackground.getSize().x/background.getScale().x, py);
            
            reached += isBlue(image, px, py) || isBlue(image, px, image.getSize().y*(destiy-disty)/tbackground.getSize().y/background.getScale().y) ||
            isBlue(image, image.getSize().x*(destix+distx)/tbackground.getSize().x/background.getScale().x, image.getSize().y*(destiy-disty)/tbackground.getSize().y/background.getScale().y) ||
            isBlue(image, image.getSize().x*(destix-distx)/tbackground.getSize().x/background.getScale().x, image.getSize().y*(destiy-disty)/tbackground.getSize().y/background.getScale().y) ||
            isBlue(image, image.getSize().x*(destix+distx)/tbackground.getSize().x/background.getScale().x, py) ||
            isBlue(image, image.getSize().x*(destix-distx)/tbackground.getSize().x/background.getScale().x, py);
            
            destix -= positions[i].x;
            destiy -= positions[i].y;
            px -= positions[i].x;
            py -= positions[i].y;
        }
        }
		if(fly || allinWhite ){
				playerPosition.x += movx;
				playerPosition.y += movy;
		}
		else {
            if (reached > 0){
                if(! tbackground.loadFromFile("pics/end.png")) std::cout << "end Image Not Loaded " << std::endl;;
                background.setTexture(tbackground);
                //background.setOrigin(background.getLocalBounds().width/2, background.getLocalBounds().height/2);
                //background.setPosition(playerPosition);
                playerPosition.x = 300;
                playerPosition.y = 300;
                positions = std::vector < sf::Vector2f > ();
    positions.push_back(sf::Vector2f(0,0));
    positions.push_back(sf::Vector2f((rand()%400)-200,(rand()%400)-200)); 
    positions.push_back(sf::Vector2f((rand()%400)-200,(rand()%400)-200)); 
    positions.push_back(sf::Vector2f((rand()%400)-200,(rand()%400)-200)); 
    positions.push_back(sf::Vector2f((rand()%400)-200,(rand()%400)-200)); 
    positions.push_back(sf::Vector2f((rand()%400)-200,(rand()%400)-200)); 
    positions.push_back(sf::Vector2f((rand()%400)-200,(rand()%400)-200)); 
    positions.push_back(sf::Vector2f((rand()%400)-200,(rand()%400)-200)); 
    positions.push_back(sf::Vector2f((rand()%400)-200,(rand()%400)-200)); 
    positions.push_back(sf::Vector2f((rand()%400)-200,(rand()%400)-200)); 
    positions.push_back(sf::Vector2f((rand()%400)-200,(rand()%400)-200)); 
    positions.push_back(sf::Vector2f((rand()%400)-200,(rand()%400)-200)); 
    positions.push_back(sf::Vector2f((rand()%400)-200,(rand()%400)-200)); 
            
                fly = true;
            }else{
                playerPosition.x = 300;
                playerPosition.y = 300;
                positions = std::vector < sf::Vector2f > ();
    positions.push_back(sf::Vector2f(0,0));
    positions.push_back(sf::Vector2f((rand()%200)-100,(rand()%200)-100));
    positions.push_back(sf::Vector2f((rand()%200)-100,(rand()%200)-100)); 
            }
        }
		//Setting movement variables to 0
		movx = movy = 0;

		//Checking the sprite sources and position to be ok
		if(spriteSource.x >= 8) spriteSource.x = 0;
		float aux = background.getTexture()->getSize().y * background.getScale().y;
		if(playerPosition.x < 0) playerPosition.x = 0;
		else if (playerPosition.y < 0) playerPosition.y = 0;
		else if (playerPosition.x > window.getSize().x) playerPosition.x = window.getSize().x-1;
		else if (playerPosition.y > aux) playerPosition.y = aux -1;

		//Clear the window
		window.clear();

		//Set player properties (some are unnecessary but is useful in case there are changes i.e. the screen changes)
		player.setOrigin(spriteSize.x/2, spriteSize.y);
		player.setPosition(playerPosition.x, playerPosition.y);
		player.setTextureRect(sf::IntRect(spriteSource.x*spriteSize.x, 
			spriteSource.y*spriteSize.y, spriteSize.x, spriteSize.y));
		player.setScale(window.getSize().x/300.0, window.getSize().x/300.0);
		player.scale(originalSpriteSize.x/spriteSize.x, originalSpriteSize.y/spriteSize.y);

		//Set background scale
		escalat = (double)(view.getSize().x) / (double)(tbackground.getSize().x);
		background.setScale(escalat, escalat);

		//Set view values
		view.reset(sf::FloatRect(playerPosition.x,playerPosition.y, 
							window.getSize().x, window.getSize().y));
		if(playerPosition.y+10 > view.getSize().y/2) 
			viewPosition.y = playerPosition.y+10;
		else viewPosition.y = view.getSize().y/2;
		viewPosition.x = view.getSize().x/2;
		view.setCenter(viewPosition);
		view.setViewport(sf::FloatRect(0,0,1.0f,1.0f));

		//Set window view, draw and display
		window.setView(view);
		window.draw(background);
        for(int i = 0; i < positions.size(); ++i){
            player.move(positions[i]);
            window.draw(player);
            player.move(-positions[i].x,-positions[i].y);
        }
		window.display();
	}
}
