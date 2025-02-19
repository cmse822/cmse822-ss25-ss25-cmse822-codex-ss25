// problems/ProblemRegistry.hpp
#pragma once // Alternatively, use include guards

#include <memory>
#include <string>

// Forward declarations of problem classes
namespace agoge
{
    namespace problems
    {
        class Problem;
        class SodShockTube;
        class GravityCollapse;
        class GaussianPulse; 
        class Sedov;

        // Declaration of the createProblem function
        std::unique_ptr<Problem> createProblem(const std::string &name);
    }
}
