//
//  AssetParser.h
//  snowcrash
//
//  Created by Zdenek Nemec on 5/17/13.
//  Copyright (c) 2013 Apiary Inc. All rights reserved.
//

#ifndef SNOWCRASH_ASSETPARSER_H
#define SNOWCRASH_ASSETPARSER_H

#include "SectionParser.h"
#include "RegexMatch.h"
#include "CodeBlockUtility.h"

namespace snowcrash {

    /// Asset signature
    enum AssetSignature {
        NoAssetSignature = 0,
        BodyAssetSignature,         /// < Explicit body asset
        ImplicitBodyAssetSignature, /// < Body asset using abbreviated syntax
        SchemaAssetSignature,       /// < Explicit Schema asset
        UndefinedAssetSignature = -1
    };

    /** Body matching regex */
    const char* const BodyRegex = "^[[:blank:]]*[Bb]ody[[:blank:]]*$";

    /** Schema matching regex */
    const char* const SchemaRegex = "^[[:blank:]]*[Ss]chema[[:blank:]]*$";

    struct AssetSectionTraits {
        static const mdp::MarkdownNodeType MarkdownNodeType = mdp::ListItemMarkdownNodeType;
        typedef EnumList<BodySectionType, SchemaSectionType>  SectionTypes;
    };

    /**
     *  Asset Section Processor
     */
    template<>
    struct SectionProcessor<Asset, AssetSectionTraits> : public SectionProcessorBase<Asset, AssetSectionTraits> {

        static MarkdownNodeIterator processSignature(const MarkdownNodeIterator& node,
                                                     const MarkdownNodes& siblings,
                                                     SectionParserData& pd,
                                                     SectionLayout& layout,
                                                     const ParseResultRef<Asset>& out) {

            out.node = "";
            CodeBlockUtility::signatureContentAsCodeBlock(node, pd, out.report, out.node);

            if (pd.exportSourceMap() && !out.node.empty()) {
                out.sourceMap.sourceMap.append(node->sourceMap);
            }

            return ++MarkdownNodeIterator(node);
        }

        static MarkdownNodeIterator processDescription(const MarkdownNodeIterator& node,
                                                       const MarkdownNodes& siblings,
                                                       SectionParserData& pd,
                                                       const ParseResultRef<Asset>& out) {

            return node;
        }

        static MarkdownNodeIterator processContent(const MarkdownNodeIterator& node,
                                                   const MarkdownNodes& siblings,
                                                   SectionParserData& pd,
                                                   const ParseResultRef<Asset>& out) {

            mdp::ByteBuffer content;
            CodeBlockUtility::contentAsCodeBlock(node, pd, out.report, content);

            out.node += content;

            if (pd.exportSourceMap() && !content.empty()) {
                out.sourceMap.sourceMap.append(node->sourceMap);
            }

            return ++MarkdownNodeIterator(node);
        }

        static bool isDescriptionNode(const MarkdownNodeIterator& node,
                                      SectionType sectionType) {
            return false;
        }

        static bool isContentNode(const MarkdownNodeIterator& node,
                                  SectionType sectionType) {

            return (SectionKeywordSignature(node) == UndefinedSectionType);
        }

    };

    /** Asset Section Parser */
    typedef SectionProcessor<Asset, AssetSectionTraits> AssetProcessor;
    typedef SectionParser<Asset, AssetSectionTraits> AssetParser;
}

#endif
