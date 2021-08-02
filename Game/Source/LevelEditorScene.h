#ifndef __LEVEL_EDITOR_H__
#define __LEVEL_EDITOR_H__

class LevelEditor
{
public:

	LevelEditor();

	~LevelEditor();

	bool Start();

	bool Update(float dt);

	bool CleanUp();
};

#endif // __LEVEL_EDITOR_H__
