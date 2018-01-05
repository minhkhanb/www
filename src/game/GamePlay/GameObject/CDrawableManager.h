#if !_DRAWABLE_MANAGER_H
#define _DRAWABLE_MANAGER_H (1)
#define DRAWABLE_POOL_MAX (1000)
class CAnimObject;
class CDrawableManager
{
	CAnimObject* _anim_list_static[DRAWABLE_POOL_MAX];
	float _posX[DRAWABLE_POOL_MAX];
	float _posY[DRAWABLE_POOL_MAX];
	float _scalex[DRAWABLE_POOL_MAX];
	float _scaley[DRAWABLE_POOL_MAX];
	float _scale_centerx[DRAWABLE_POOL_MAX];
	float _scale_centery[DRAWABLE_POOL_MAX];
	float _rotationAngle[DRAWABLE_POOL_MAX];
	float _rotationX[DRAWABLE_POOL_MAX];
	float _rotationY[DRAWABLE_POOL_MAX];	
	int _ids[DRAWABLE_POOL_MAX];
	int _opacity[DRAWABLE_POOL_MAX];

	int _num;
public: 
	
	void Add(CAnimObject* object, int opacity, float posX, float posY, float scalex, float scaley, float centerx, float centery, float rotationAngle, float rotationX, float rotationY);
	void Add(CAnimObject* object, int opacity, float posX, float posY, float scalex, float scaley, float centerx, float center);
	void Add(CAnimObject* object, int opacity, float posX, float posY);

	void Update();
	void Render(float x,float y);
	void Init();
};


#endif