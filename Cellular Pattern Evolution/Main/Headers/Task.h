#include <string>
#include "../../Lattice/Headers/Lattice.h"
#include "../../ObjectiveFunctions/Headers/ObjectiveFunction.h"
#include "../../Optimizer/Headers/Optimizer.h"

namespace task
{
	class task
	{
	private:
		std::unique_ptr<optimizer::optimizer> _optimizer;
		std::shared_ptr<objective_functions::ca_multiobj_func> _obj_func;
		std::shared_ptr<lattice::lattice> _lattice;
	public:
		task(std::string configPath);
		void execute();
	};
}