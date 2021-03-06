// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/MeasureWriter.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/impl/Converter.h"
#include "mx/core/elements/Backup.h"
#include "mx/core/elements/Forward.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Clef.h"
#include "mx/core/elements/Directive.h"
#include "mx/core/elements/Divisions.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Instruments.h"
#include "mx/core/elements/Key.h"
#include "mx/core/elements/KeyChoice.h"
#include "mx/core/elements/TraditionalKey.h"
#include "mx/core/elements/Cancel.h"
#include "mx/core/elements/Fifths.h"
#include "mx/core/elements/Mode.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/MeasureStyle.h"
#include "mx/core/elements/PartSymbol.h"
#include "mx/core/elements/StaffDetails.h"
#include "mx/core/elements/Staves.h"
#include "mx/core/elements/Time.h"
#include "mx/core/elements/Transpose.h"
#include "mx/core/elements/TimeChoice.h"
#include "mx/core/elements/TimeSignatureGroup.h"
#include "mx/core/elements/BeatType.h"
#include "mx/core/elements/Beats.h"
#include "mx/core/elements/Clef.h"
#include "mx/core/elements/ClefOctaveChange.h"
#include "mx/core/elements/Line.h"
#include "mx/core/elements/Sign.h"
#include "mx/impl/ScoreWriter.h"
#include "mx/core/elements/Print.h"
#include "mx/core/elements/LayoutGroup.h"
#include "mx/core/elements/MeasureLayout.h"
#include "mx/core/elements/MeasureNumbering.h"
#include "mx/core/elements/PartAbbreviationDisplay.h"
#include "mx/core/elements/PartNameDisplay.h"
#include "mx/core/elements/SystemLayout.h"
#include "mx/core/elements/SystemDistance.h"
#include "mx/core/elements/SystemDividers.h"
#include "mx/core/elements/SystemMargins.h"
#include "mx/core/elements/TopSystemDistance.h"
#include "mx/core/elements/LeftMargin.h"
#include "mx/core/elements/RightMargin.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/Accidental.h"
#include "mx/core/elements/Beam.h"
#include "mx/core/elements/CueNoteGroup.h"
#include "mx/core/elements/Dot.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/EditorialVoiceGroup.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/FullNoteTypeChoice.h"
#include "mx/core/elements/GraceNoteGroup.h"
#include "mx/core/elements/Grace.h"
#include "mx/core/elements/Cue.h"
#include "mx/core/elements/Instrument.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/Lyric.h"
#include "mx/core/elements/Notations.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/Notehead.h"
#include "mx/core/elements/NoteheadText.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/Pitch.h"
#include "mx/core/elements/Play.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/Stem.h"
#include "mx/core/elements/Rest.h"
#include "mx/core/elements/Tie.h"
#include "mx/core/elements/TimeModification.h"
#include "mx/core/elements/Type.h"
#include "mx/core/elements/Unpitched.h"
#include "mx/core/elements/Voice.h"
#include "mx/impl/NoteWriter.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/core/elements/AccordionRegistration.h"
#include "mx/core/elements/Bracket.h"
#include "mx/core/elements/Coda.h"
#include "mx/core/elements/Damp.h"
#include "mx/core/elements/DampAll.h"
#include "mx/core/elements/Dashes.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/core/elements/Eyeglasses.h"
#include "mx/core/elements/HarpPedals.h"
#include "mx/core/elements/Image.h"
#include "mx/core/elements/Metronome.h"
#include "mx/core/elements/OctaveShift.h"
#include "mx/core/elements/OtherDirection.h"
#include "mx/core/elements/Pedal.h"
#include "mx/core/elements/Percussion.h"
#include "mx/core/elements/PrincipalVoice.h"
#include "mx/core/elements/Rehearsal.h"
#include "mx/core/elements/Scordatura.h"
#include "mx/core/elements/Segno.h"
#include "mx/core/elements/StringMute.h"
#include "mx/core/elements/Wedge.h"
#include "mx/core/elements/Words.h"
#include "mx/impl/DirectionWriter.h"
#include "mx/core/elements/Barline.h"
#include "mx/core/elements/BarStyle.h"
#include "mx/core/elements/Coda.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/Ending.h"
#include "mx/core/elements/Fermata.h"
#include "mx/core/elements/Repeat.h"
#include "mx/core/elements/Segno.h"
#include "mx/core/elements/WavyLine.h"

#include <limits>

namespace mx
{
    
    
    namespace core
    {
        class PartwiseMeasure;
        using PartwiseMeasurePtr = std::shared_ptr<PartwiseMeasure>;
    }

    namespace impl
    {
        MeasureWriter::MeasureWriter( const api::MeasureData& inMeasureData, const MeasureCursor& inCursor, const ScoreWriter& inScoreWriter )
        : myMeasureData{ inMeasureData }
        , myOutMeasure{ nullptr }
        , myCursor{ inCursor }
        , myPreviousCursor{ inCursor }
        , myScoreWriter{ inScoreWriter }
        , myPropertiesWriter{ nullptr }
        , myConverter{}
        , myBarlinesIter{ inMeasureData.barlines.cbegin() }
        , myBarlinesEnd{ inMeasureData.barlines.cend() }
        , myMeasureKeysIter{ inMeasureData.keys.cbegin() }
        , myMeasureKeysEnd{ inMeasureData.keys.cend() }
        {

        }


        core::PartwiseMeasurePtr MeasureWriter::getPartwiseMeasure()
        {
            myOutMeasure = core::makePartwiseMeasure();
            myPropertiesWriter = std::unique_ptr<PropertiesWriter>{ new PropertiesWriter{ myOutMeasure } };
            myCursor.reset();
            myBarlinesIter = myMeasureData.barlines.cbegin();
            myBarlinesEnd = myMeasureData.barlines.cend();
            myMeasureKeysIter = myMeasureData.keys.cbegin();
            
            writeMeasureGlobals();
            writeStaves();
            myPropertiesWriter->flushBuffer();
            writeBarlines( api::TICK_TIME_INFINITY );
            myPropertiesWriter = nullptr;
            return myOutMeasure;
        }
        
        void MeasureWriter::writeMeasureGlobals()
        {
            writeBarlines( 0 );
            auto& measureAttr = *myOutMeasure->getAttributes();
            
            if( myMeasureData.number.size() > 0 )
            {
                measureAttr.number = core::XsToken{ myMeasureData.number };
            }
            else
            {
                measureAttr.number = core::XsToken{ std::to_string( myCursor.measureIndex + 1 ) };
            }
            
            if( myMeasureData.width >= 0.0 )
            {
                measureAttr.hasWidth = true;
                measureAttr.width = core::PositiveDivisionsValue{ myMeasureData.width };
            }
            
            Converter converter;
            
            if( myMeasureData.implicit != api::Bool::unspecified )
            {
                measureAttr.hasImplicit = true;
                measureAttr.implicit = converter.convert( myMeasureData.implicit );
            }
            
            if( myScoreWriter.isStartOfSystem( myCursor.measureIndex ) )
            {
                writeSystemInfo();
            }
            
            if( myCursor.isFirstMeasureInPart )
            {
                myPropertiesWriter->writeDivisions( myCursor.getGlobalTicksPerQuarter() );
                
                if( myMeasureData.staves.size() > 1 )
                {
                    myPropertiesWriter->writeNumStaves( static_cast<int>( myMeasureData.staves.size() ) );
                }
            }
            
            if( !myMeasureData.timeSignature.isImplicit )
            {
                myPropertiesWriter->writeTime( myMeasureData.timeSignature );
            }
            
            int localStaffCounter = 0;
            for( const auto staff : myMeasureData.staves )
            {
                auto clefIter = staff.clefs.cbegin();
                auto clefEnd = staff.clefs.cend();
                while( clefIter != clefEnd && clefIter->tickTimePosition == 0 )
                {
                    int desiredStaffIndex = -1;
                    if( myCursor.getNumStaves() > 1 )
                    {
                        desiredStaffIndex = localStaffCounter;
                    }
                    myPropertiesWriter->writeClef( desiredStaffIndex, *clefIter );
                    ++clefIter;
                }
                ++localStaffCounter;
            }
            
            // TODO - these might end up in the wrong place, figure out how
            // to preserve location if the barline is not at the beginning or
            // the end
            
            while( myMeasureKeysIter != myMeasureKeysEnd && myMeasureKeysIter->tickTimePosition == 0 )
            {
                myPropertiesWriter->writeKey( -1, *myMeasureKeysIter );
                ++myMeasureKeysIter;
            }
        }


        void MeasureWriter::writeSystemInfo()
        {
            auto systemData = myScoreWriter.getSystemData( myCursor.measureIndex );
            
            const bool isSystemDataSpecified =
                   systemData.isMarginSpecified
                || systemData.isSystemDistanceSpecified
                || systemData.isTopSystemDistanceSpecified;
            
            if( !isSystemDataSpecified && !myCursor.isFirstMeasureInPart )
            {
                return;
            }
            
            auto printMdc = core::makeMusicDataChoice();
            printMdc->setChoice( core::MusicDataChoice::Choice::print );
            auto& print = *printMdc->getPrint();
            print.getAttributes()->hasNewSystem = true;
            print.getAttributes()->newSystem = core::YesNo::yes;
            auto& layoutGroup = *print.getLayoutGroup();
            myOutMeasure->getMusicDataGroup()->addMusicDataChoice(printMdc );
            if( isSystemDataSpecified )
            {
                layoutGroup.setHasSystemLayout( true );
                auto& systemLayout = *layoutGroup.getSystemLayout();

                if( systemData.isMarginSpecified )
                {
                    systemLayout.setHasSystemMargins( true );
                    auto& margins = *systemLayout.getSystemMargins();
                    margins.getLeftMargin()->setValue( core::TenthsValue{static_cast<core::DecimalType>( systemData.leftMargin ) } );
                    margins.getRightMargin()->setValue( core::TenthsValue{static_cast<core::DecimalType>( systemData.rightMargin ) } );
                }

                if( systemData.isTopSystemDistanceSpecified )
                {
                    systemLayout.setHasTopSystemDistance( true );
                    systemLayout.getTopSystemDistance()->setValue( core::TenthsValue{static_cast<core::DecimalType>( systemData.topSystemDistance ) } );
                }

                if( systemData.isSystemDistanceSpecified )
                {
                    systemLayout.setHasSystemDistance( true );
                    systemLayout.getSystemDistance()->setValue( core::TenthsValue{static_cast<core::DecimalType>( systemData.systemDistance ) } );
                }
           }
        }
        
        
        void MeasureWriter::writeStaves()
        {
            myCursor.tickTimePosition = 0;
            myCursor.staffIndex = 0;
            myCursor.voiceIndex = 0;
            myPreviousCursor = myCursor;
            for( const auto& staff : myMeasureData.staves )
            {
                // auto clefIter = staff.clefs.cbegin();
                // auto clefEnd = staff.clefs.cend();
                
                writeVoices( staff );
                ++myCursor.staffIndex;
            }
        }
        
        
        void MeasureWriter::writeVoices( const api::StaffData& inStaff )
        {
            auto clefIter = inStaff.clefs.cbegin();
            const auto clefEnd = inStaff.clefs.cend();
            while( clefIter != clefEnd && clefIter->tickTimePosition == 0 ) { ++clefIter; }
//            auto myMeasureKeysIter = myMeasureKeys.cbegin();
//            auto myMeasureKeysEnd = myMeasureKeys.cend();
            auto staffKeyIter = inStaff.keys.cbegin();
            auto staffKeyEnd = inStaff.keys.cend();
            auto directionIter = inStaff.directions.cbegin();
            auto directionEnd = inStaff.directions.cend();
            
            for( const auto& voice : inStaff.voices )
            {
                int currentChordTickPosition = -1;
                int previousChordTickPosition = -2;
                myCursor.voiceIndex = voice.first;
                auto noteIter = voice.second.notes.cbegin();
                auto noteEnd = voice.second.notes.cend();
                for( ; noteIter != noteEnd; ++noteIter )
                {
                    // TODO - this is too simplistic. If we have two chords in a row then this
                    // will be incorrect
                    bool isStartOfChord = false;
                    myCursor.isChordActive = noteIter->isChord;
                    
                    if ( noteIter->isChord )
                    {
                        currentChordTickPosition = noteIter->tickTimePosition;
                        if( currentChordTickPosition != previousChordTickPosition )
                        {
                            isStartOfChord = true;
                        }
                        previousChordTickPosition = currentChordTickPosition;
                    }
                    
                    const auto& apiNote = *noteIter;
                    writeForwardOrBackupIfNeeded( apiNote );
                    
                    if( myMeasureKeysIter != myMeasureKeysEnd )
                    {
                        if( myMeasureKeysIter->tickTimePosition <= myCursor.tickTimePosition )
                        {
                            myPropertiesWriter->writeKey( -1, *myMeasureKeysIter );
                            ++myMeasureKeysIter;
                        }
                    }
                    
                    while( clefIter != clefEnd && clefIter->tickTimePosition <= myCursor.tickTimePosition )
                    {
                        myPropertiesWriter->writeClef( myCursor.staffIndex, *clefIter );
                        ++clefIter;
                    }
                    for( ; staffKeyIter != staffKeyEnd && staffKeyIter->tickTimePosition <= myCursor.tickTimePosition; ++staffKeyIter )
                    {
                        myPropertiesWriter->writeKey( myCursor.staffIndex, *staffKeyIter );
                    }
                    myPropertiesWriter->flushBuffer();
                    
                    for( ; directionIter != directionEnd && directionIter->tickTimePosition <= myCursor.tickTimePosition; ++directionIter )
                    {
                        if( isDirectionDataEmpty( *directionIter ) )
                        {
                            continue;
                        }
                        auto directionMdc = core::makeMusicDataChoice();
                        directionMdc->setChoice( core::MusicDataChoice::Choice::direction );
                        DirectionWriter directionWriter{ *directionIter, myCursor };
                        directionMdc->setDirection( directionWriter.getDirection() );
                        myOutMeasure->getMusicDataGroup()->addMusicDataChoice( directionMdc );
                    }
                    
                    auto mdc = core::makeMusicDataChoice();
                    mdc->setChoice( core::MusicDataChoice::Choice::note );
                    NoteWriter writer{ apiNote, myCursor, myScoreWriter, myPreviousCursor.isChordActive };
                    mdc->setNote( writer.getNote(isStartOfChord) );
                    myOutMeasure->getMusicDataGroup()->addMusicDataChoice( mdc );
                    advanceCursorIfNeeded( apiNote, noteIter, noteEnd, isStartOfChord );
                } // foreach note
            } // foreach voice
            
            bool areClefsRemaining = clefIter != clefEnd;
            bool areMeasureKeysRemaining = myMeasureKeysIter != myMeasureKeysEnd;
            bool areStaffKeysRemaining = staffKeyIter != staffKeyEnd;
            
            if( areClefsRemaining || areMeasureKeysRemaining || areStaffKeysRemaining )
            {
                for( ; clefIter != inStaff.clefs.cend(); ++clefIter )
                {
                    MX_ASSERT( clefIter != inStaff.clefs.cend() );
                    api::ClefData clef = *clefIter;
                    myPropertiesWriter->writeClef( myCursor.staffIndex, clef );
                }
                
                if( myMeasureKeysIter != myMeasureKeysEnd )
                {
                    if( myMeasureKeysIter->tickTimePosition )
                    {
                        myPropertiesWriter->writeKey( -1, *myMeasureKeysIter );
                        ++myMeasureKeysIter;
                    }
                }
                
                for( ; staffKeyIter != staffKeyEnd; ++staffKeyIter )
                {
                    myPropertiesWriter->writeKey( myCursor.staffIndex, *staffKeyIter );
                }
                myPropertiesWriter->flushBuffer();
            }
            
            for( ; directionIter != directionEnd; ++directionIter )
            {
                if( isDirectionDataEmpty( *directionIter ) )
                {
                    continue;
                }
                auto directionMdc = core::makeMusicDataChoice();
                directionMdc->setChoice( core::MusicDataChoice::Choice::direction );
                DirectionWriter directionWriter{ *directionIter, myCursor };
                directionMdc->setDirection( directionWriter.getDirection() );
                myOutMeasure->getMusicDataGroup()->addMusicDataChoice( directionMdc );
            }
            
        } // func writeVoices
        
        
        void MeasureWriter::writeForwardOrBackupIfNeeded( const api::NoteData& currentNote )
        {
            myPropertiesWriter->flushBuffer();
            const int timeDifference = currentNote.tickTimePosition - myCursor.tickTimePosition;
            if( timeDifference < 0 )
            {
                backup( -1 * timeDifference );
            }
            else if ( timeDifference > 0 )
            {
                forward( timeDifference );
            }
        }
        
        
        void MeasureWriter::backup( const int ticks )
        {
            auto backupMdc = core::makeMusicDataChoice();
            backupMdc->setChoice( core::MusicDataChoice::Choice::backup );
            auto backup = backupMdc->getBackup();
            backup->getDuration()->setValue( core::PositiveDivisionsValue{ static_cast<core::DecimalType>( ticks ) } );
            myOutMeasure->getMusicDataGroup()->addMusicDataChoice( backupMdc );
            myCursor.tickTimePosition -= ticks;
        }
        
        
        void MeasureWriter::forward( const int ticks )
        {
            auto forwardMdc = core::makeMusicDataChoice();
            forwardMdc->setChoice( core::MusicDataChoice::Choice::forward );
            auto forward = forwardMdc->getForward();
            forward->getDuration()->setValue( core::PositiveDivisionsValue{ static_cast<core::DecimalType>( ticks ) } );
            myOutMeasure->getMusicDataGroup()->addMusicDataChoice( forwardMdc );
            myCursor.tickTimePosition += ticks;
        }
        
        
        void MeasureWriter::advanceCursorIfNeeded( const api::NoteData& currentNote, NoteIter inNoteIter, const NoteIter inEndIter, bool isStartOfChord )
        {
            MX_UNUSED(currentNote);
            MX_UNUSED(isStartOfChord);
            
            ++inNoteIter;
            if( inNoteIter != inEndIter )
            {
                myCursor.tickTimePosition = inNoteIter->tickTimePosition;
            }
            else
            {
                myCursor.tickTimePosition += currentNote.durationData.durationTimeTicks;
                myCursor.isChordActive = true;
            }

            myPreviousCursor = myCursor;
        }
        
        
        void MeasureWriter::writeBarlines( int tickTimePosition )
        {
            for( ; myBarlinesIter != myBarlinesEnd && myBarlinesIter->tickTimePosition <= tickTimePosition; ++myBarlinesIter )
            {
                auto mdc = core::makeMusicDataChoice();
                mdc->setChoice( core::MusicDataChoice::Choice::barline );
                auto barlineElement = mdc->getBarline();
                if( myBarlinesIter->barlineType != api::BarlineType::unspecified && myBarlinesIter->barlineType != api::BarlineType::unsupported )
                {
                    barlineElement->setHasBarStyle( true );
                    barlineElement->getBarStyle()->setValue( myConverter.convert( myBarlinesIter->barlineType ) );
                }
                if( myBarlinesIter->location != api::HorizontalAlignment::unspecified )
                {
                    barlineElement->getAttributes()->hasLocation = true;
                    barlineElement->getAttributes()->location = myConverter.convertBarlinePlacement( myBarlinesIter->location );
                }
                myOutMeasure->getMusicDataGroup()->addMusicDataChoice( mdc );
            }
        }
    }
}
