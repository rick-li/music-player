/*
 * Raging MIDI (https://github.com/waddlesplash/ragingmidi).
 *
 * Copyright (c) 2012-2013 WaddleSplash & contributors (see AUTHORS.txt).
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall
 * be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <QElapsedTimer>
#include <QMidi.h>

#include "Player.h"
//#include "Gui/MainWind.h"

Player::Player(QMidiFile* fil, qint32 tick)
    : QThread(0)
{
    f = fil;
//    piano = p;
    doStop = false;
    sTick = tick;
}

void Player::handleEvent()
{
//    if((MainWind::trackStatus->value(e->track(),true) == false) && e->isNoteEvent())
//    { return; }

    if(e->type() == QMidiEvent::SysEx) { // TODO: sysex
    } else {
        qint32 message = e->message();
        QMidi::outSendMsg(message);
    }

    // Update the piano, pianoroll, and slider
    if(e->type() == QMidiEvent::NoteOn) {
//        emit addTrackColor(e->note(),e->track());
        emit tickChanged(e->tick());
    } else if(e->type() == QMidiEvent::NoteOff) {
//        emit removeTrackColor(e->note(),e->track());
    } else if(e->type() == QMidiEvent::ControlChange) {
//        emit clearTrackColors(e->track());
    }
}

void Player::run()
{
    float sTime = 0; /* start time, in seconds */
    if(sTick) { sTime = f->timeFromTick(sTick); }
    QElapsedTimer t;
    t.start();
    foreach(e,f->events()) {
        if(e->isNoteEvent() && (e->tick() < sTick)) { continue; }

        if(e->type() != QMidiEvent::Meta) {
            qint64 event_time = (f->timeFromTick(e->tick())-sTime) * 1000;

            qint32 waitTime = event_time - t.elapsed();
            if(waitTime > 0) {
                msleep(waitTime);
            }
            handleEvent();
        }

        if(doStop)
        { return; }
    }
}
