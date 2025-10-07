#pragma once

#include "GameObject.h"

class RootJob : public GameObject
{
public:
	RootJob();
	~RootJob();

	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Release() override;
};
