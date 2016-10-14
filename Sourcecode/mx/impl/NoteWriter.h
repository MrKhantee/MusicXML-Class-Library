// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/NoteData.h"
#include "mx/impl/MeasureCursor.h"
#include "mx/impl/Converter.h"

namespace mx
{
    namespace core
    {
        class Note;
        using NotePtr = std::shared_ptr<Note>;
        class NoteChoice;
        using NoteChoicePtr = std::shared_ptr<NoteChoice>;
        class FullNoteGroup;
        using FullNoteGroupPtr = std::shared_ptr<FullNoteGroup>;
        class FullNoteTypeChoice;
        using FullNoteTypeChoicePtr = std::shared_ptr<FullNoteTypeChoice>;
    }
    
    namespace impl
    {
        class ScoreWriter;
        
        class NoteWriter
        {
        public:
            NoteWriter( const api::NoteData& inNoteData, const MeasureCursor& inCursor, const ScoreWriter& inScoreWriter );
            core::NotePtr getNote() const;
            
        private:
            const api::NoteData& myNoteData;
            const impl::MeasureCursor& myCursor;
            const impl::ScoreWriter& myScoreWriter;
            const Converter myConverter;
            mutable core::NotePtr myOutNote;
            mutable core::NoteChoicePtr myOutNoteChoice;
            mutable core::FullNoteGroupPtr myOutFullNoteGroup;
            mutable core::FullNoteTypeChoicePtr myOutFullNoteTypeChoice;
            
        private:
            void setNoteChoiceAndFullNoteGroup() const;
            void setFullNoteTypeChoice() const;
            void setStaffAndVoice() const;
            void setDurationNameAndDots() const;
            void setStemDirection() const;

        };
    }
}