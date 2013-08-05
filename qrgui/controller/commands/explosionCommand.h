#pragma once

#include "abstractCommand.h"
#include "../../models/logicalModelAssistApi.h"
#include "../../models/graphicalModelAssistApi.h"

namespace qReal {
namespace commands {

/// Adds or removes explosion between specified elements
class ExplosionCommand : public AbstractCommand
{
public:
	/// Creates new instance of a command that creates or removes explosions.
	/// If graphical assist api parameter specified then it is allowed to
	/// pass graphical ids as source or target parameters.
	/// Otherwise both source and target must be logical ids.
	/// @param mustAdd Specifies if this command should create or remove explosions.
	ExplosionCommand(models::LogicalModelAssistApi * const logicalApi
			, models::GraphicalModelAssistApi const * const graphicalApi
			, Id const &source, Id const &target, bool mustAdd);
	virtual ~ExplosionCommand();

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	bool processExplosion(bool add);

	models::LogicalModelAssistApi *mLogicalApi;
	models::GraphicalModelAssistApi const *mGraphicalApi;
	Id mSource;
	Id mTarget;
	bool const mMustAdd;
};

}
}
