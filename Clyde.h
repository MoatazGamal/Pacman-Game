#pragma once

#include<SFML/Graphics.hpp>
#include"Animation.h"
#include<sstream>
#ifndef Clyde_H

#define Clyde_H

class Clyde:public Blinky
{
public:
	sf::Sprite s_Clyde;
	enum class AnimationIndex
	{
		downStanding,
		upDirection,
		downDirection,
		rightDirection,
		leftDirection,
		frightened,
		back,
		die,
		Count
	};
	Clyde(const sf::Vector2f& position)
	{
		s_Clyde.setTextureRect({ 0,96,32,32 });
		s_Clyde.setPosition(position);
		animations[int(AnimationIndex::downStanding)] = Animation(64, 96, 32, 32, 1, 10.4f, path);
		animations[int(AnimationIndex::upDirection)] = Animation(0, 96, 32, 32, 2, 0.4f, path);
		animations[int(AnimationIndex::downDirection)] = Animation(64, 96, 32, 32, 2, 0.4f, path);
		animations[int(AnimationIndex::leftDirection)] = Animation(128, 96, 32, 32, 2, 0.4f, path);
		animations[int(AnimationIndex::rightDirection)] = Animation(192, 96, 32, 32, 2, 0.4f, path);
		animations[int(AnimationIndex::frightened)] = Animation(0, 128, 32, 32, 2, 0.4f, path);
		animations[int(AnimationIndex::back)] = Animation(0, 128, 32, 32, 3, 0.4f, path);
		animations[int(AnimationIndex::die)] = Animation(128, 128, 32, 32, 3, 0.4f, path);
		movespeed = 2;
		x = 450;
		y = 420;

	}
	void Draw(sf::RenderWindow& window) const
	{
		window.draw(s_Clyde);


	}
	void Update(float deltaTime, sf::Vector2f Pacman, sf::Sprite& clyde)
	{
		prevP = Gpos;
		animations[int(currentAnimation)].Update(deltaTime);
		animations[int(currentAnimation)].ApplyToSprite(s_Clyde);
		Gpos = FindPath(Pacman, clyde);
		if (Gpos.x > 0)
		{
			currentAnimation = AnimationIndex::rightDirection;
		}
		else if (Gpos.x < 0)
		{
			currentAnimation = AnimationIndex::leftDirection;
		}
		else if (Gpos.y < 0)
		{
			currentAnimation = AnimationIndex::upDirection;
		}
		else if (Gpos.y > 0)
		{
			currentAnimation = AnimationIndex::downDirection;
		}
		int Grow = int(y / 30);
		int Gcol = int(x / 30);

		//These conditions prevent ghosts Backtracking.
		if (prevP.y != 0 && (Gpos.y*-1) == (prevP.y))
		{
			Gpos.y = prevP.y;
			if (node.wall[Grow - 1][Gcol] == 1 || node.wall[Grow + 1][Gcol] == 1)
			{
				Gpos.y = 0;
				if (node.wall[Grow][Gcol - 1] != 1)
				{
					Gpos.x = -1;
				}
				else if (node.wall[Grow][Gcol + 1] != 1)
				{
					Gpos.x = 1;
				}
			}
			if (currentAnimation == AnimationIndex::upDirection)
			{
				currentAnimation = AnimationIndex::downDirection;
			}
			else
			{
				currentAnimation = AnimationIndex::upDirection;
			}

		}
		if (prevP.x != 0 && (Gpos.x*-1) == (prevP.x))
		{
			Gpos.x = prevP.x;
			if (node.wall[Grow][Gcol + 1] == 1 || node.wall[Grow][Gcol - 1] == 1)
			{
				Gpos.x = 0;
				if (node.wall[Grow - 1][Gcol] != 1)
				{
					Gpos.y = -1;
				}
				else if (node.wall[Grow + 1][Gcol] != 1)
				{
					Gpos.y = 1;
				}
			}
			if (currentAnimation == AnimationIndex::rightDirection)
			{
				currentAnimation = AnimationIndex::leftDirection;
			}
			else
			{
				currentAnimation = AnimationIndex::rightDirection;
			}
		}
		dirmove(Gpos);
		moving();
		s_Clyde.setPosition(x, y);
	}
	void Update5(float deltaTime, sf::Sprite& clyde)
	{
		clyde.setPosition(435, 420);
		x = 450;
		y = 420;
		walking = false;
		move[UP] = false;
		move[DOWN] = false;
		move[LEFT] = false;
		move[RIGHT] = false;
		prevP = {};
		currentAnimation = AnimationIndex::upDirection;
		animations[int(currentAnimation)].Update(deltaTime);
		animations[int(currentAnimation)].ApplyToSprite(clyde);
	}
	void Frightened_Mode(bool superDot_eaten ,sf::Time time,bool eaten)
	{
		if (superDot_eaten == true && time.asSeconds()<13)
		{
			currentAnimation = AnimationIndex::frightened;
		}
		if (eaten == true)
		{
			currentAnimation = AnimationIndex::die;
		}
		else if (superDot_eaten == true && time.asSeconds() > 13)
		{
			currentAnimation = AnimationIndex::back;
		}
	}
private:

	Animation animations[int(AnimationIndex::Count)];
	AnimationIndex currentAnimation = AnimationIndex::downStanding;
	std::string path = "Resources/Graphics/Ghost.png";
	sf::Vector2f Gpos;
	sf::Vector2f prevP = {};
	Map node;
};



#endif