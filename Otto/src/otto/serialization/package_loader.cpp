#include "package_loader.h"

#include "otto/debug/log.h"
#include "otto/serialization/serializer.h"

namespace otto
{
    Result<Package, PackageLoader::PackageLoadingError> 
        PackageLoader::loadPackage(const Serialized& serialized, const FilePath& coreRootDirectory)
    {
        Log::trace("Loading package...");

        Package package;
        FilePath rootDirectory = serialized.contains("rootDirectory") ? serialized.get<String>("rootDirectory") : "";

        if (serialized.contains("components"))
        {
            auto components = serialized.get("components");
            if (components.isList())
                package.components = components.getList<String>();
        }

        if (serialized.contains("behaviours"))
        {
            auto behaviours = serialized.get("behaviours");
            if (behaviours.isList())
                package.behaviours = behaviours.getList<String>();
        }

        if (serialized.contains("systems"))
        {
            auto systems = serialized.get("systems");
            if (systems.isList())
                package.systems = systems.getList<String>();
        }

        if (serialized.contains("events"))
        {
            auto events = serialized.get("events");
            if (events.isList())
                package.events = events.getList<String>();
        }

        if (serialized.contains("dependencies"))
        {
            auto dependencies = serialized.get("dependencies");
            if (dependencies.isList())
            {
                for (auto& dependencyName : dependencies.getList<String>())
                {
                    FilePath dependencyPath;
                    if (dependencyName.startsWith("otto/"))
                        dependencyPath = coreRootDirectory + "packages/" + dependencyName;
                    else
                    {
                        if (!serialized.contains("rootDirectory"))
                        {
                            Log::error("Failed to load Package: Root Directory not found");
                            return PackageLoadingError::ROOT_DIRECTORY_NOT_FOUND;
                        }

                        dependencyPath = rootDirectory + dependencyName;
                    }

                    auto dependencySerialized = Serializer::load(dependencyPath.toString().endsWith(".otto") ? dependencyPath : dependencyPath + ".otto");
                    if (dependencySerialized.hasError())
                    {
                        Log::error("Failed to load package: Syntax error");
                        return PackageLoadingError::SYNTAX_ERROR;
                    }

                    auto dependency = loadPackage(dependencySerialized.getResult(), coreRootDirectory);

                    if (dependency.hasError())
                        return dependency.getError();
                    else
                    {
                        package.components.add(dependency.getResult().components);
                        package.behaviours.add(dependency.getResult().behaviours);
                        package.systems.add(dependency.getResult().systems);
                        package.events.add(dependency.getResult().events);
                    }
                }
            }
        }

        return package;
    }

} // namespace otto
