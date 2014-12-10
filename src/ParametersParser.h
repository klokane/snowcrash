//
//  ParametersParser.h
//  snowcrash
//
//  Created by Zdenek Nemec on 9/1/13.
//  Copyright (c) 2013 Apiary Inc. All rights reserved.
//

#ifndef SNOWCRASH_PARAMETERSPARSER_H
#define SNOWCRASH_PARAMETERSPARSER_H

#include "SectionParser.h"
#include "ParameterParser.h"
#include "RegexMatch.h"
#include "StringUtility.h"
#include "BlueprintUtility.h"

namespace snowcrash {

    /** Parameters matching regex */
    const char* const ParametersRegex = "^[[:blank:]]*[Pp]arameters?[[:blank:]]*$";

    struct ParametersSectionTraits {
        static const mdp::MarkdownNodeType MarkdownNodeType = mdp::ListItemMarkdownNodeType;
        typedef EnumList<ParametersSectionType> SectionTypes;
    };

    /** No parameters specified message */
    const char* const NoParametersMessage = "no parameters specified, expected a nested list of parameters, one parameter per list item";

    /** Internal type alias for Collection iterator of Parameter */
    typedef Collection<Parameter>::iterator ParameterIterator;

    /**
     * Parameters section processor
     */
    template<>
    struct SectionProcessor<Parameters, ParametersSectionTraits> : public SectionProcessorBase<Parameters, ParametersSectionTraits> {

        static MarkdownNodeIterator processSignature(const MarkdownNodeIterator& node,
                                                     const MarkdownNodes& siblings,
                                                     SectionParserData& pd,
                                                     SectionLayout& layout,
                                                     const ParseResultRef<Parameters>& out) {

            mdp::ByteBuffer remainingContent;

            GetFirstLine(node->text, remainingContent);

            if (!remainingContent.empty()) {

                // WARN: Extra content in parameters section
                std::stringstream ss;
                ss << "ignoring additional content after 'parameters' keyword,";
                ss << " expected a nested list of parameters, one parameter per list item";

                mdp::CharactersRangeSet sourceMap = mdp::BytesRangeSetToCharactersRangeSet(node->sourceMap, pd.sourceData);
                out.report.warnings.push_back(Warning(ss.str(),
                                                      IgnoringWarning,
                                                      sourceMap));
            }

            return ++MarkdownNodeIterator(node);
        }

        static MarkdownNodeIterator processDescription(const MarkdownNodeIterator& node,
                                                       const MarkdownNodes& siblings,
                                                       SectionParserData& pd,
                                                       const ParseResultRef<Parameters>& out) {

            return node;
        }

        static MarkdownNodeIterator processNestedSection(const MarkdownNodeIterator& node,
                                                         const MarkdownNodes& siblings,
                                                         SectionParserData& pd,
                                                         const ParseResultRef<Parameters>& out) {

            if (pd.sectionContext() != ParameterSectionType) {
                return node;
            }

            IntermediateParseResult<Parameter> parameter(out.report);

            ParameterParser::parse(node, siblings, pd, parameter);

            if (!out.node.empty()) {

                ParameterIterator duplicate = findParameter(out.node, parameter.node);

                if (duplicate != out.node.end()) {

                    // WARN: Parameter already defined
                    std::stringstream ss;
                    ss << "overshadowing previous parameter '" << parameter.node.name << "' definition";

                    mdp::CharactersRangeSet sourceMap = mdp::BytesRangeSetToCharactersRangeSet(node->sourceMap, pd.sourceData);
                    out.report.warnings.push_back(Warning(ss.str(),
                                                          RedefinitionWarning,
                                                          sourceMap));
                }
            }

            out.node.push_back(parameter.node);

            if (pd.exportSourceMap()) {
                out.sourceMap.collection.push_back(parameter.sourceMap);
            }

            return ++MarkdownNodeIterator(node);
        }

        static bool isDescriptionNode(const MarkdownNodeIterator& node,
                                      SectionType sectionType) {

            return false;
        }

        static SectionType nestedSectionType(const MarkdownNodeIterator& node) {

            return ParameterProcessor::sectionType(node);
        }

        static SectionTypes nestedSectionTypes() {
            SectionTypes nested;

            // Parameter & descendants
            nested.push_back(ParameterSectionType);
            SectionTypes types = ParameterProcessor::nestedSectionTypes();
            nested.insert(nested.end(), types.begin(), types.end());

            return nested;
        }

        static void finalize(const MarkdownNodeIterator& node,
                             SectionParserData& pd,
                             const ParseResultRef<Parameters>& out) {

            if (out.node.empty()) {

                // WARN: No parameters defined
                mdp::CharactersRangeSet sourceMap = mdp::BytesRangeSetToCharactersRangeSet(node->sourceMap, pd.sourceData);
                out.report.warnings.push_back(Warning(NoParametersMessage,
                                                      FormattingWarning,
                                                      sourceMap));
            }
        }

        /** Finds a parameter inside a parameters collection */
        static ParameterIterator findParameter(Parameters& parameters,
                                               const Parameter& parameter) {

            return std::find_if(parameters.begin(),
                                parameters.end(),
                                std::bind2nd(MatchName<Parameter>(), parameter));
        }
    };

    /** Parameters Section parser */
    typedef SectionProcessor<Parameters, ParametersSectionTraits> ParametersProcessor;
    typedef SectionParser<Parameters, ParametersSectionTraits> ParametersParser;
}

#endif
