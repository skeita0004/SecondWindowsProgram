#include "RootJob.h"
#include "SceneManager.h"

RootJob::RootJob() :
	GameObject(nullptr, "RootJob")
{
}

RootJob::~RootJob()
{
}

void RootJob::Init()
{
	Instantiate<SceneManager> (this);
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
