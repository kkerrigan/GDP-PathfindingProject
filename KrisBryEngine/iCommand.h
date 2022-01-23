#ifndef _Command_HG_
#define _Command_HG_

class iCommand {
public:
	virtual ~iCommand() = default;
	virtual void update(float deltaTime) = 0;
	virtual bool isFinished() = 0;
};

#endif
