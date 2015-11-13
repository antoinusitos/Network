#pragma once

namespace sf
{
	class Shape;
}

namespace engine
{
	namespace gameplay
	{
		class PlayerEntity
		{
		public:
			PlayerEntity();
			~PlayerEntity();

			void update();
			void updatePosition(float X, float Y, int theID);
			void setOriginal(bool newBool);
			void setID(int newID);
			void Fire();
			int getID();
		private:
			sf::Shape *_shape;
			float _x;
			float _y;
			int ID;
			int IDParent;
			bool original;
		};
	}
}
