#include "Button.h"
#include "Sprite.h"

//Base constructor
Button::Button(Sprite* graphics)
{
	m_Sprite = graphics;
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_Width = 0;
	m_Height = 0;
	m_Sprite->setCurrentClip(BUTTON_SPRITE_MOUSE_OUT);
}

void Button::setPosition(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

void Button::handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < m_Pos.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > m_Pos.x + m_Width)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < m_Pos.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > m_Pos.y + m_Height)
		{
			inside = false;
		}

		//Mouse is outside button
		if (!inside)
		{
			m_Sprite->setCurrentClip(BUTTON_SPRITE_MOUSE_OUT);
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				m_Sprite->setCurrentClip(BUTTON_SPRITE_MOUSE_OVER_MOTION);
				break;

			case SDL_MOUSEBUTTONDOWN:
				m_Sprite->setCurrentClip(BUTTON_SPRITE_MOUSE_DOWN);
				break;

			case SDL_MOUSEBUTTONUP:
				m_Sprite->setCurrentClip(BUTTON_SPRITE_MOUSE_UP);
				break;
			}
		}
	}
}

void Button::render(SDL_Renderer* renderer)
{
	//Show current button sprite
	m_Sprite->render(m_Pos.x, m_Pos.y, m_Sprite->getCurrentClip());
}
