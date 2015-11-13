#pragma once

namespace sf
{
	class Shape;
}

namespace engine
{
	namespace gameplay
	{
		class BulletEntity
		{
		public:
			BulletEntity();
			~BulletEntity();

			void update();
			void updatePosition(float X, float Y, int theID);
			void setVelocity(float dirX, float dirY);
			void setID(int newID);
			int getID();
			void setPosition(float theX, float theY);
		private:
			sf::Shape *_shape;
			float _x;
			float _y;
			float dx;
			float dy;
			int ID;
			bool original;

		};
	}
}