#pragma once

#include "otto/base.h"
#include "otto/util/result.h"
#include "otto/serialization/serialized.h"
#include "otto/util/platform/file_path.h"

namespace otto
{
    class Serializer
    {
    public:
        enum class ParsingError
        {
            INDENTATION_ERROR,
            SYNTAX_ERROR_CLOSED_CURLY_BRACKETS,
            SYNTAX_ERROR_CLOSED_SQUARE_BRACKETS,
            SYNTAX_ERROR_MISSING_COLON,
            SYNTAX_ERROR_MORE_THAN_ONE_COLON,
            SYNTAX_ERROR_ILLEGAL_COLON,
            SYNTAX_ERROR,
        };

        static Result<Serialized, ParsingError> load(const FilePath& filePath);

    private:
        struct SerializableParsingResult
        {
            uint64 linesParsed;
            Serialized serialized;
        };

        static Result<SerializableParsingResult, ParsingError> _parseSerializable(
            DynamicArray<String>& lines, uint64 lineIndex, uint64 outerIndentation);

        static Result<SerializableParsingResult, Serializer::ParsingError> _parseList(
            Serialized& serialized, DynamicArray<String>& lines, uint64 lineIndex, uint64 outerIndentation);
        static Result<SerializableParsingResult, Serializer::ParsingError> _parseDictionary(
            Serialized& serialized, DynamicArray<String>& lines, uint64 lineIndex, uint64 outerIndentation);
    };

} // namespace otto
