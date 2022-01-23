#ifndef _cCommandGroup_HG_
#define _cCommandGroup_HG_

#include "iCommand.h"

#include <string>
#include <vector>

class cCommandGroup {
public:
	cCommandGroup(const std::string& name, bool isParallel);
	~cCommandGroup();
	void update(double deltaTime);
	bool isFinished() const;

	//contained commands and command groups
	std::vector<iCommand*> commands;
	std::vector<cCommandGroup*> commandGroups;

	std::string name;
	bool runParallel;

private:
	bool finished;
};

#endif
