#include "pch.h"
#include "../../GameCore/CGame.h"

void CDrawableManager::Add(CAnimObject* object, int opacity, float posX, float posY, float scalex, float scaley, float centerx, float centery, float rotationAngle, float rotationX, float rotationY)
{
	_anim_list_static[_num] = object;
	_posX[_num] = posX;
	_posY[_num] = posY;
	_scalex[_num] = scalex;
	_scaley[_num] = scaley;
	_scale_centerx[_num] = centerx;
	_scale_centery[_num] = centery;
	_rotationAngle[_num] = rotationAngle;
	_rotationX[_num] = rotationX;
	_rotationY[_num] = rotationY;
	_opacity[_num] = opacity;
	//short
	int index = _num;
	for(int i = 0; i < _num; i++)
	{
		int node = _ids[i];
		if(posY < _posY[node])
		{
            index = i;
			if(!(_posY[node] == posY && _posX[node] < posX))
			{
				index = i;
				break;
			}
		}
	}
    //printf("\nindex = %d",index);
	for(int i = _num-1; i >= index; i--)
	{
		_ids[i+1] = _ids[i];
	}
	_ids[index] = _num;
	_num++;
	
}
void CDrawableManager::Add(CAnimObject* object, int opacity, float posX, float posY, float scalex, float scaley, float centerx, float centery)
{
	_anim_list_static[_num] = object;
	_posX[_num] = posX;
	_posY[_num] = posY;
	_scalex[_num] = scalex;
	_scaley[_num] = scaley;
	_scale_centerx[_num] = centerx;
	_scale_centery[_num] = centery;
	_rotationAngle[_num] = 0;
	_rotationX[_num] = posX;
	_rotationY[_num] = posY;
	_opacity[_num] = opacity;
	//short
	int index = _num;
	for (int i = 0; i < _num; i++)
	{
		int node = _ids[i];
		if (posY < _posY[node])
		{
			index = i;
			if (!(_posY[node] == posY && _posX[node] < posX))
			{
				index = i;
				break;
			}
		}
	}
	//printf("\nindex = %d",index);
	for (int i = _num - 1; i >= index; i--)
	{
		_ids[i + 1] = _ids[i];
	}
	_ids[index] = _num;
	_num++;

}
void CDrawableManager::Add(CAnimObject* object, int opacity, float posX, float posY)
{
	_anim_list_static[_num] = object;
	_posX[_num] = posX;
	_posY[_num] = posY;

	_scalex[_num] = 1;
	_scaley[_num] = 1;
	_scale_centerx[_num] = posX;
	_scale_centery[_num] = posY;
	_rotationAngle[_num] = 0;
	_rotationX[_num] = posX;
	_rotationY[_num] = posY;
	_opacity[_num] = opacity;

	//short
	int index = _num;
	for (int i = 0; i < _num; i++)
	{
		int node = _ids[i];
		if (posY < _posY[node])
		{
			index = i;
			if (!(_posY[node] == posY && _posX[node] < posX))
			{
				index = i;
				break;
			}
		}
	}
	//printf("\nindex = %d",index);
	for (int i = _num - 1; i >= index; i--)
	{
		_ids[i + 1] = _ids[i];
	}
	_ids[index] = _num;
	_num++;

}
void CDrawableManager::Update()
{
}
void CDrawableManager::Render(float x,float y)
{
	float posX = 0;
	float posY = 0;
	int index = 0;
	//printf("\n_num  = %d",_num);
	for(int i = 0; i < _num; i++)
	{
		 index = _ids[i];
		 posX = _posX[index] + x;
		 posY = _posY[index] + y;
		 G()->SetScale(_scalex[index], _scaley[index], _scale_centerx[index] + x, _scale_centery[index] + y);
		 G()->SetRotate(_rotationAngle[index], _rotationX[index] + x, _rotationY[index] + y);
		 G()->SetOpacity(_opacity[index]);
		 _anim_list_static[index]->SetPos(posX, posY);
		 _anim_list_static[index]->DrawAnimObject(G());
		 G()->SetOpacity(100);
		 G()->ClearRotate();
		 G()->ClearScale();		 
	}
}
void CDrawableManager::Init()
{
	_num = 0;
}