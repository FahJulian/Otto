#include "profiler.h"

namespace otto
{
    Profiler::FunctionProfiler::FunctionProfiler(Function& function)
		: mFunction(function), mStartTime(Time::getTime64())
	{
	}

	Profiler::FunctionProfiler::~FunctionProfiler()
	{
		mFunction.runs++;
		mFunction.duration += Time::getTime64() - mStartTime;
	}
	
	Profiler* Profiler::get()
	{
		static Profiler profiler;
		return &profiler;
	}

	Profiler::FunctionProfiler Profiler::createFunctionProfiler(const String& functionName)
	{
		if (!get()->mFunctions.containsKey(functionName))
			get()->mFunctions.insert(functionName, Function());

		return FunctionProfiler(get()->mFunctions[functionName]);
	}

	void Profiler::logResults()
	{
		Log::trace("Profiler results:\n");

		for (auto& [name, function] : get()->mFunctions)
		{
			Log::trace("Function ", name, "():");
			Log::trace("    Ran: ", function.runs, (function.runs == 1 ? " time" : " times"));
			Log::trace("    Total duration: ", function.duration * 1000, "ms");
			Log::trace("    Average duration: ", function.duration / function.runs * 1000, "ms");
			Log::trace('\n');
		}
	}
}
