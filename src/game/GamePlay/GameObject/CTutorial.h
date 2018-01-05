#if !_CTUTORIAL_H
#define _CTUTORIAL_H 1
class CTutorial
{
public:
	CTutorial(){};

	char _string[1024];
	BYTE _state;
	BYTE _step;
	
	void SetTutorial(BYTE state);

	void UpdateTutorial();
	void RenderTutorial(float x, float y);

private:

};

#endif