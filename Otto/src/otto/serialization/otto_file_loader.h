#pragma once

#include "otto/base.h"
#include "otto/util/result.h"
#include "otto/serialization/otto_file.h"
#include "otto/util/platform/file_path.h"

namespace otto
{
    class OttoFileLoader
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

        static Result<OttoFile, ParsingError> load(const FilePath& filePath);

    private:
        struct SerializableParsingResult
        {
            uint64 linesParsed;
            Serialized serialized;
        };

        static void _filterEmtpyLines(DynamicArray<String>& lines);

        static Result<DictioaryParsingResult, ParsingError> _parseDictionary(
            DynamicArray<String>& lines, uint64 keyIndex, uint64 keyIndentation, Serialized::Type type = Serialized::Type::VOID);

        static Result<SerializableParsingResult, ParsingError> _parseSerializable(DynamicArray<String>& lines, uint64 lineIndex, uint64 outerIndentation);
    };

} // namespace otto
