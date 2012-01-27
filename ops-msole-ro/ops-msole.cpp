// ops-msole
#include "ops-msole.h"

// ops
#include "ops-log.h"


namespace ops
{
namespace msole
{
    bool init() {
		DEBUG << "libops-msole-ro initialized.";
		return true;
	}

	void shutdown() {
		DEBUG << "libops-msole-ro shutdown.";
	}
}
}
