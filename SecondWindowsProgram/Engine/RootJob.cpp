#include "RootJob.h"
#include "../Game/PlayScene.h"

RootJob::RootJob()
{
}

RootJob::~RootJob()
{
}

void RootJob::Init()
{
	Instantiate<PlayScene> (this);
}

void RootJob::Update()
{
}

void RootJob::Draw()
{
}

void RootJob::Release()
{
}
