#pragma once

#include "otto/base.h"
#include "otto/util/map.h"
#include "otto/util/string.h"
#include "otto/debug/log/log.h"
#include "otto/core/platform/time.h"

namespace otto
{
	class Profiler
	{
	private:
		struct Function
		{
			int32 runs;
			float64 duration;
		};

	public:
		class FunctionProfiler
		{
			FunctionProfiler(Function& function);

		public:
			~FunctionProfiler();

		private:
			Function& mFunction;
			float64 mStartTime;

			friend class Profiler;
		};

	private:
		Profiler()
		{
		}

		static Profiler* get();

		~Profiler()
		{
		}

	public:
		static FunctionProfiler createFunctionProfiler(const String& functionName);

		static void logResults();

	private:
		Map<String, Function> mFunctions;
	};

} // namespace otto

#ifdef OTTO_PROFILE
#define OTTO_PROFILE_FUNCTION(functionName) otto::Profiler::FunctionProfiler __functionProfiler = otto::Profiler::createFunctionProfiler(functionName)
#else
#define OTTO_PROFILE_FUNCTION(functionName)
#endif
