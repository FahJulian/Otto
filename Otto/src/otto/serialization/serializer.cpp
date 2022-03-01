#include "serializer.h"

#include "otto/util/file.h"
#include "otto/debug/log/log.h"

namespace otto
{
    namespace
    {
        void _filterEmtpyLines(DynamicArray<String>& lines)
        {
            DynamicArray<uint64> emptyLines;
            for (uint64 i = 0; i < lines.getSize(); i++)
            {
                auto& line = lines.get(i);
                line.toSubString(0, line.findFirstOf("//"));     // Get rid of comments

                if (line.findFirstNotOfWhiteSpace() == line.getSize())      // Line is empty
                    emptyLines.add(i);
            }

            for (uint64 i = 0; i < emptyLines.getSize(); i++)
                lines.remove(emptyLines.get(i) - i);
        }

        Serialized _determineType(const DynamicArray<String>& lines, const uint64 lineIndex)
        {
            Serialized serialized = Serialized(Serialized::Type::VOID, false);

            if (String::trim(lines[lineIndex]).endsWith('{'))
                return Serialized(Serialized::Type::DICTIONARY, true);
            else if (String::trim(lines[lineIndex]).endsWith('['))
                return Serialized(Serialized::Type::LIST, true);
            else if (String::trim(lines[lineIndex]).endsWith(':') && lines.getSize() >= (lineIndex + 1) + 1)
                return lines[lineIndex + 1].contains(':') ? Serialized(Serialized::Type::DICTIONARY) : Serialized(Serialized::Type::LIST);

            return serialized;
        }

    } // namespace

    Result<Serialized, Serializer::ParsingError> Serializer::load(const FilePath& filePath)
    {
        Serialized serialized = Serialized(Serialized::Type::DICTIONARY, false, true);
        DynamicArray<String> lines = File(filePath).readLines();

        _filterEmtpyLines(lines);

        for (uint64 i = 0; i < lines.getSize(); i++)
        {
            String& line = lines.get(i);
            uint64 indentation = line.findFirstNotOfWhiteSpace();
            line.trim();

            if (indentation != 0)
                return ParsingError::INDENTATION_ERROR;

            uint64 seperatorIndex = line.findFirstOf(':');
            if (seperatorIndex == line.getSize())
            {
                Log::error("Syntax error in line ", i, ": Missing colon.");
                return ParsingError::SYNTAX_ERROR_MISSING_COLON;
            }
            else if (line.findFirstOf(": ", seperatorIndex + 1) != line.getSize())
            {
                Log::error("Syntax error in line ", i, ": More than one colon.");
                return ParsingError::SYNTAX_ERROR_MORE_THAN_ONE_COLON;
            }

            String key = String::subString(line, 0, seperatorIndex).trim();

            if (line.endsWith(':') || line.endsWith('{') || line.endsWith('['))
            {
                auto result = _parseSerializable(lines, i, indentation);

                if (result.hasError())
                    return result.getError();

                serialized.mDictionary.insert(key, result.getResult().serialized);
                i += result.getResult().linesParsed;
            }
            else
                serialized.mDictionary.insert(key, String::subString(line, seperatorIndex + 1).trim());
        }
        
        return serialized;
    }

    Result<Serializer::SerializableParsingResult, Serializer::ParsingError> Serializer::_parseSerializable(
        DynamicArray<String>& lines, uint64 lineIndex, uint64 outerIndentation)
    {
        Serialized serialized = _determineType(lines, lineIndex);

        if (serialized.mType == Serialized::Type::DICTIONARY)
            return _parseDictionary(serialized, lines, lineIndex, outerIndentation);
        else if (serialized.mType == Serialized::Type::LIST)
            return _parseList(serialized, lines, lineIndex, outerIndentation);

        return ParsingError::SYNTAX_ERROR;
    }

    Result<Serializer::SerializableParsingResult, Serializer::ParsingError> 
        Serializer::_parseDictionary(Serialized& serialized, DynamicArray<String>& lines, uint64 lineIndex, uint64 outerIndentation)
    {
        if (lines.getSize() == lineIndex + 1)
            return Serializer::SerializableParsingResult{ 1, serialized };

        uint64 innerIndentation = lines[lineIndex + 1].findFirstNotOfWhiteSpace();

        for (uint64 i = lineIndex + 1; i < lines.getSize(); i++)
        {
            String line = lines.get(i);
            uint64 indentation = line.findFirstNotOfWhiteSpace();

            if (String::trim(line).startsWith('}'))
            {
                if (indentation == outerIndentation)
                    return SerializableParsingResult{ i - lineIndex, serialized };
                else
                {
                    Log::error("Syntax error in line ", i, ": Indentation does not match outer indentation.");
                    return ParsingError::INDENTATION_ERROR;
                }
            }
            else if (indentation <= outerIndentation)
                return SerializableParsingResult{ i - (lineIndex + 1), serialized };
            else
            {
                line.trim();

                if (indentation != innerIndentation)
                {
                    Log::error("Syntax error in line ", i, ": Indentation does not match inner indentation.");
                    return ParsingError::INDENTATION_ERROR;
                }

                uint64 seperatorIndex = line.findFirstOf(':');
                if (seperatorIndex == line.getSize())
                {
                    Log::error("Syntax error in line ", i, ": Missing colon.");
                    return ParsingError::SYNTAX_ERROR_MISSING_COLON;
                }
                else if (line.findFirstOf(": ", seperatorIndex + 1) != line.getSize())
                {
                    Log::error("Syntax error in line ", i, ": More than one colon");
                    return ParsingError::SYNTAX_ERROR_MORE_THAN_ONE_COLON;
                }
                else
                {
                    String key = String::subString(line, 0, seperatorIndex).trim();

                    if (line.endsWith(':') || line.endsWith('{') || line.endsWith('['))
                    {
                        auto result = _parseSerializable(lines, i, indentation);

                        if (result.hasError())
                            return result.getError();

                        serialized.mDictionary.insert(key, result.getResult().serialized);
                        i += result.getResult().linesParsed;
                    }
                    else
                    {
                        serialized.mDictionary.insert(key, String::subString(line, seperatorIndex + 1).trim());
                    }
                }
            }
        }

        return SerializableParsingResult{ lines.getSize() - (lineIndex + 1), serialized };
    }

    Result<Serializer::SerializableParsingResult, Serializer::ParsingError>
        Serializer::_parseList(Serialized& serialized, DynamicArray<String>& lines, uint64 lineIndex, uint64 outerIndentation)
    {
        if (lines.getSize() == lineIndex + 1)
            return SerializableParsingResult{ 1, serialized };

        uint64 innerIndentation = lines[lineIndex + 1].findFirstNotOfWhiteSpace();

        for (uint64 i = lineIndex + 1; i < lines.getSize(); i++)
        {
            auto& line = lines.get(i);
            uint64 indentation = line.findFirstNotOfWhiteSpace();

            if (String::trim(line).startsWith(']'))
            {
                if (indentation == outerIndentation)
                    return SerializableParsingResult{ i - lineIndex, serialized };
                else
                {
                    Log::error("Syntax error in line ", i, ": Indentation does not match outer indentation");
                    return ParsingError::INDENTATION_ERROR;
                }
            }
            else if (indentation <= outerIndentation)
                return SerializableParsingResult{ i - (lineIndex + 1), serialized };
            else
            {
                line.trim();

                if (indentation != innerIndentation)
                {
                    Log::error("Syntax error in line ", i, ": Indentation does not match outer indentation");
                    return ParsingError::INDENTATION_ERROR;
                }

                if (line.contains(':'))
                {
                    Log::error("Syntax error in line ", i, ": Illegal colon");
                    return ParsingError::SYNTAX_ERROR_ILLEGAL_COLON;
                }
                else
                {
                    if (line.endsWith(':') || line.endsWith('{') || line.endsWith('['))
                    {
                        auto result = _parseSerializable(lines, i, indentation);

                        if (result.hasError())
                            return result.getError();

                        serialized.mList.add(result.getResult().serialized);
                        i += result.getResult().linesParsed;
                    }
                    else
                    {
                        serialized.mList.add(line);
                    }
                }
            }
        }

        return SerializableParsingResult{ lines.getSize() - (lineIndex + 1), serialized };
    }

} // namespace otto
