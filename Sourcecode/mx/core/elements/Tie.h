// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/TieAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( TieAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Tie )

        inline TiePtr makeTie() { return std::make_shared<Tie>(); }

        class Tie : public ElementInterface
        {
        public:
            Tie();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TieAttributesPtr getAttributes() const;
            void setAttributes( const TieAttributesPtr& attributes );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            TieAttributesPtr myAttributes;
        };
    }
}
