#include "otto_file_loader.h"

#include "otto/util/file.h"

namespace otto
{
    Result<OttoFile, OttoFileLoader::ParsingError> OttoFileLoader::load(const FilePath& filePath)
    {
        OttoFile file;
        DynamicArray<String> lines = File(filePath).readLines();

        _filterEmtpyLines(lines);

        for (uint64 i = 0; i < lines.getSize(); i++)
        {
            String& line = lines.get(i);
            uint64 indentation = line.findFirstNotOfWhiteSpace();
            line.trim();

            if (line.startsWith('#'))
            {
                auto words = String::split(String::subString(line, 1), ' ');

                if (words.getSize() >= 2 && words[0].equalsIgnoreCase("TYPE") && words[1].equalsIgnoreCase("SCENE"))
                    file.type = OttoFile::Type::SCENE;
            }
            else
            {
                if (indentation != 0)
                    return ParsingError::INDENTATION_ERROR;

                uint64 seperatorIndex = line.findFirstOf(':');
                if (seperatorIndex == line.getSize())
                    return ParsingError::SYNTAX_ERROR_MISSING_COLON;
                else if (line.findFirstOf(':', seperatorIndex + 1) != line.getSize())
                    return ParsingError::SYNTAX_ERROR_MORE_THAN_ONE_COLON;

                String key = String::subString(line, 0, seperatorIndex).trim();

                auto result = _parseSerializable(lines, i, indentation);

                if (result.hasError())
                    return result.getError();

                file.serialized.mDictionary.insert(key, result.getResult().serialized);
                i += result.getResult().linesParsed;
            }
        }

        return file;
    }

    void OttoFileLoader::_filterEmtpyLines(DynamicArray<String>& lines)
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

    Result<OttoFileLoader::SerializableParsingResult, OttoFileLoader::ParsingError> OttoFileLoader::_parseSerializable(
        DynamicArray<String>& lines, uint64 lineIndex, uint64 outerIndentation)
    {
        Serialized serialized = Serialized(Serialized::Type::VOID);

        if (String::trim(lines[lineIndex]).endsWith('{'))
            serialized.mType = Serialized::Type::DICTIONARY;
        else if (String::trim(lines[lineIndex]).endsWith('['))
            serialized.mType = Serialized::Type::LIST;
        else if (String::trim(lines[lineIndex]).endsWith(':') && lines.getSize() >= (lineIndex + 1) + 1)
            serialized.mType = lines[lineIndex + 1].contains(':') ? Serialized::Type::DICTIONARY : Serialized::Type::LIST;

        if (serialized.mType == Serialized::Type::DICTIONARY)
        {
            if (lines.getSize() == lineIndex + 1)
                return SerializableParsingResult{ 1, serialized };

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
                        return ParsingError::INDENTATION_ERROR;
                }
                else if (indentation <= outerIndentation)
                    return SerializableParsingResult{ i - (lineIndex + 1), serialized };
                else
                {
                    line.trim();

                    if (indentation != innerIndentation)
                        return ParsingError::INDENTATION_ERROR;

                    uint64 seperatorIndex = line.findFirstOf(':');
                    if (seperatorIndex == line.getSize())
                        return ParsingError::SYNTAX_ERROR_MISSING_COLON;
                    else if (line.findFirstOf(':', seperatorIndex + 1) != line.getSize())
                        return ParsingError::SYNTAX_ERROR_MORE_THAN_ONE_COLON;
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
        else if (serialized.mType == Serialized::Type::LIST)
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
                        return ParsingError::INDENTATION_ERROR;
                }
                else if (indentation <= outerIndentation)
                    return SerializableParsingResult{ i - (lineIndex + 1), serialized };
                else
                {
                    line.trim();

                    if (indentation != innerIndentation)
                        return ParsingError::INDENTATION_ERROR;

                    if (line.contains(':'))
                        return ParsingError::SYNTAX_ERROR_ILLEGAL_COLON;
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

        return ParsingError::SYNTAX_ERROR;
    }

} // namespace otto
