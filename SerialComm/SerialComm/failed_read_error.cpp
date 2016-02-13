#include "failed_read_error.h"

failed_read_error::failed_read_error() : runtime_error(
	"ERROR: Attempted serial read returned null.")
{
	// Nothing else to do
}
