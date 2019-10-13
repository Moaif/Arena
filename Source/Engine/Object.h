#pragma once
#include "RTTI.h"
#include "..//Globals.h"

class Object{
	RTTI_ENABLE_BASE(Object);
public:
	virtual bool Init() { return true; };
	virtual bool Start() { return true; };
	virtual update_status PreUpdate() { return UPDATE_CONTINUE; };
	virtual update_status Update() { return UPDATE_CONTINUE; };
	virtual bool CleanUp() { return true; };

	virtual const std::string& GetName() const { return m_name; };
	virtual void SetName(const std::string& name) { m_name = name; };
	virtual bool IsReadyToDelete()const { return m_toDelete; };
	virtual void SetToDelete(bool value = true) { m_toDelete = value; };

private:
	std::string m_name;
	bool m_toDelete = false;
};