#ifndef __MODULE_H__
#define __MODULE_H__

class Application;

class Module
{
public:

	Module(bool active = true) : m_active(active)
	{}

	virtual ~Module()
	{}

	bool IsEnabled() const
	{
		return m_active;
	}

	 bool Enable()
	{
		if(m_active == false)
			return m_active = Start();

		return true;
	}

	 bool Disable()
	 {
		 if(m_active == true)
			 return m_active = !CleanUp();

		 return true;
	 }

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Restart()
	{
		return true;
	}

	virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}


private:
	bool m_active = true;
};

#endif // __MODULE_H__