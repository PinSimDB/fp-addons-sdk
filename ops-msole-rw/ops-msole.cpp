// ops-msole
#include "ops-msole.h"

// ops
#include "ops-log.h"

// gsf
#include <gsf/gsf-utils.h>

#include <gsf/gsf-input-stdio.h>
#include <gsf/gsf-infile.h>
#include <gsf/gsf-infile-stdio.h>
#include <gsf/gsf-infile-msole.h>

#include <gsf/gsf-output-stdio.h>
#include <gsf/gsf-outfile.h>
#include <gsf/gsf-outfile-stdio.h>
#include <gsf/gsf-outfile-msole.h>


namespace ops
{
namespace msole
{
    bool init() {
		// init libgsf
		gsf_init();
		DEBUG << "libgsf initialized.";

		DEBUG << "libops-msole-rw initialized.";
		return true;
	}

	void shutdown() {
		// shutdown libgsf
		gsf_shutdown();
		DEBUG << "libgsf shutdown.";

		DEBUG << "libops-msole-rw shutdown.";
	}
}
}
