#include "toad.h"
#include "Clicker.h"
#include "logger.h"

void c_clicker::reset_vars()
{
    //reset
    this->fonce = false;
    this->type2counter = 0;
    this->type2counter2 = 0;
    this->type2drop2 = false;
    this->type2boost = false;
    this->type2boost2 = false;
    this->type2counterboost = 0;
    this->type2counterboost2 = 0;
    this->first_click = false;
    this->inconsistensy = false;
    this->inconsistensy2 = false;
    this->inconsistensycounter = 0;
    this->inconsistensycounter2 = 0;
}

void c_clicker::thread(){

    while (toad::is_running) {
        //cpu
        if (!GetAsyncKeyState(VK_LBUTTON) || !toad::clicker::enabled) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }

        if (toad::clicker::enabled) {
            if (toad::minecraft_window == NULL) {
                toad::minecraft_window = FindWindowA("LWJGL", NULL);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }

            if (toad::clicker::slot_whitelist && !toad::clicker::whitelisted_slots[toad::clicker::curr_slot]) {std::this_thread::sleep_for(std::chrono::milliseconds(50)); continue; } 
            if (toad::clicker::rmb_lock && !GetAsyncKeyState(VK_RBUTTON)) this->can_stop = true;
            if (!this->can_stop) { std::this_thread::sleep_for(std::chrono::milliseconds(50)); continue; }
            if (!toad::clicker::inventory && toad::clicker::cursor_visible) { std::this_thread::sleep_for(std::chrono::milliseconds(50)); continue; }

            if (GetForegroundWindow() == toad::minecraft_window)
            {
                
                POINT pt;
                
                //windows 2004/2h20
                float delaymin2 = ((1000 / toad::clicker::maxcps) / 2) - 1.f;
                float delaymax2 = ((1000 / toad::clicker::mincps) / 2) - 1.f;

                float delayclick2 = toad::random_float(delaymin2 - 0.6f, delaymax2 + 1.f);
                float sometingdelay = toad::random_float(this->min2, this->max2);

                if (!fonce)
                {
                    this->min2 = delaymin2;
                    this->max2 = delaymax2;
                    this->fonce = true;
                }

                if (!GetAsyncKeyState(VK_LBUTTON))
                {
                    //reset vars
                    this->reset_vars();
                }

                if (GetAsyncKeyState(VK_LBUTTON))
                {
                    //delay on first click
                    if (!this->first_click)
                        std::this_thread::sleep_for(std::chrono::milliseconds(int(delayclick2) + 40));
                }

                if (GetAsyncKeyState(VK_LBUTTON))
                {
                    GetCursorPos(&pt);
                    this->first_click = true;
                    if (this->inconsistensy) {
                        sometingdelay += toad::random_int(30.f, 100.f);
                        this->inconsistensy = false;
                    }
                    else {
                        sometingdelay = toad::random_float(this->min2, this->max2);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds((int)sometingdelay));
                    PostMessage(toad::minecraft_window, WM_LBUTTONDOWN, MKF_LEFTBUTTONDOWN, LPARAM((pt.x, pt.y)));
                    //if (module::leftclicker::clicksounds)
                    //    PlaySound(TEXT("mouseclick.wav"), NULL, SND_FILENAME | SND_ASYNC);

                    if (toad::jitter::enable) toad::jitter::do_jitter();

                    if (toad::clicker::rmb_lock && GetAsyncKeyState(VK_RBUTTON) && can_stop) { this->can_stop = false; continue; }

                    if (this->inconsistensy2) {
                        sometingdelay += toad::random_int(30.f, 100.f);
                        this->inconsistensy2 = false;
                    }
                    else {
                        sometingdelay = toad::random_float(this->min2, this->max2);
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds((int)sometingdelay));
                    PostMessage(toad::minecraft_window , WM_LBUTTONUP, 0, LPARAM((pt.x, pt.y)));

                    if (toad::jitter::enable) toad::jitter::do_jitter();

                    this->inconsistensycounter2++;
                    this->inconsistensycounter++;
                    this->type2counter++;
                    this->type2counterboost++;

                    if (this->inconsistensycounter2 > 35) {
                        if (toad::random_int(0, 100) < 6) { this->inconsistensy2 = true;  this->inconsistensycounter2 = 0; }
                    }
                    if (this->inconsistensycounter > 25) {
                        if (toad::random_int(0, 100) < 6) { this->inconsistensy = true;  this->inconsistensycounter = 0; }
                    }

                    if (this->type2counterboost > toad::random_float(this->mincanboostchance, 25)) {
                        if (!this->type2drop2 && !this->type2counter2 > 0) {
                            this->type2boost = true;
                            this->mincanboostchance = 15;
                        }
                    }
                    if (this->type2boost) {
                        if (!toad::clicker::higher_cps) { this->mincandropchance -= 0.5f; }
                        else if (toad::clicker::higher_cps) { this->mincandropchance -= 0.1f; }

                        this->type2counterboost = 0;
                        this->min2 -= 1.4f;
                        this->max2 += 0.6f;
                        this->type2counterboost2++;
                    }
                    if (this->type2counterboost2 > 5) {
                        this->type2counterboost = 0;
                        this->type2boost = false;
                        this->type2counterboost2++;
                        if (type2counterboost2 > 45) {
                            this->min2 += 1.4f;
                            this->max2 -= 0.6f;
                            if (this->type2counterboost2 > 50) {
                                this->type2counterboost = 0;
                                this->type2counterboost2 = 0;
                                this->type2counter = 0;
                                this->type2counter2 = 0;
                            }
                        }
                    }
                    //drop
                    if (this->type2counter > toad::random_float(this->mincandropchance, 20)) {
                        if (!this->type2boost && !this->type2counterboost2 > 0) {
                            this->type2drop2 = true;
                            this->mincandropchance = 15;
                        }
                    }
                    if (type2drop2) {
                        if (toad::clicker::higher_cps) { this->mincanboostchance -= 0.5f; }
                        else if (!toad::clicker::higher_cps) { this->mincanboostchance -= 0.1f; }
                        this->type2counter = 0;
                        this->max2 += 1.6f;
                        this->type2counter2++;
                    }
                    if (this->type2counter2 > 5) {
                        this->type2counter = 0;
                        this->type2drop2 = false;
                        this->type2counter2++;
                        if (this->type2counter2 > 45) {
                            this->max2 -= 1.6f;
                            if (this->type2counter2 > 50) {
                                this->type2counter = 0;
                                this->type2counter2 = 0;
                                this->type2counterboost = 0;
                                this->type2counterboost2 = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}

void c_right_clicker::thread()
{
    while (toad::is_running) {
        //cpu
        if (!GetAsyncKeyState(VK_RBUTTON) || !toad::clicker::r::right_enabled) { std::this_thread::sleep_for(std::chrono::milliseconds(50)); }

        if (toad::clicker::r::right_enabled) {
            if (toad::minecraft_window == NULL && !toad::clicker::enabled) {
                toad::minecraft_window = FindWindowA("LWJGL", NULL);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }

            if (!toad::clicker::r::right_inventory && toad::clicker::cursor_visible) { std::this_thread::sleep_for(std::chrono::milliseconds(50)); continue; }

            if (GetForegroundWindow() == toad::minecraft_window)
            {
                POINT pt;

                //windows 2004/2h20
                float delaymin2 = ((1000 / toad::clicker::r::right_maxcps) / 2) - 1.f;
                float delaymax2 = ((1000 / toad::clicker::r::right_mincps) / 2) - 1.f;

                float delayclick2 = toad::random_float(delaymin2 - 0.6f, delaymax2 + 1.f);
                float sometingdelay = toad::random_float(this->min2, this->max2);
                if (!fonce)
                {
                    this->min2 = delaymin2;
                    this->max2 = delaymax2;
                    this->fonce = true;
                }

                if (!GetAsyncKeyState(VK_RBUTTON))
                {
                    //reset vars
                    this->reset_vars();
                }

                if (GetAsyncKeyState(VK_RBUTTON))
                {
                    //delay on first click
                    if (!this->first_click)
                        std::this_thread::sleep_for(std::chrono::milliseconds(int(delayclick2) + 40));
                }

                if (GetAsyncKeyState(VK_RBUTTON))
                {
                    GetCursorPos(&pt);
                    this->first_click = true;
                    if (this->inconsistensy) {
                        sometingdelay += toad::random_int(30.f, 100.f);
                        this->inconsistensy = false;
                    }
                    else {
                        sometingdelay = toad::random_float(this->min2, this->max2);
                    }


                    std::this_thread::sleep_for(std::chrono::milliseconds((int)sometingdelay));
                    PostMessage(toad::minecraft_window, WM_RBUTTONDOWN, MKF_RIGHTBUTTONDOWN, LPARAM((pt.x, pt.y)));

                    //if (module::leftclicker::clicksounds)
                    //    PlaySound(TEXT("mouseclick.wav"), NULL, SND_FILENAME | SND_ASYNC);

                    if (toad::jitter::enable) toad::jitter::do_jitter();

                    if (this->inconsistensy2) {
                        sometingdelay += toad::random_int(30.f, 100.f);
                        this->inconsistensy2 = false;
                    }
                    else {
                        sometingdelay = toad::random_float(this->min2, this->max2);
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds((int)sometingdelay));
                    PostMessage(toad::minecraft_window, WM_RBUTTONUP, 0, LPARAM((pt.x, pt.y)));

                    if (toad::jitter::enable) toad::jitter::do_jitter();

                    this->inconsistensycounter2++;
                    this->inconsistensycounter++;
                    this->type2counter++;
                    this->type2counterboost++;

                    if (this->inconsistensycounter2 > 35) {
                        if (toad::random_int(0, 100) < 6) { this->inconsistensy2 = true;  this->inconsistensycounter2 = 0; }
                    }
                    if (this->inconsistensycounter > 25) {
                        if (toad::random_int(0, 100) < 6) { this->inconsistensy = true;  this->inconsistensycounter = 0; }
                    }

                    if (this->type2counterboost > toad::random_float(this->mincanboostchance, 25)) {
                        if (!this->type2drop2 && !this->type2counter2 > 0) {
                            this->type2boost = true;
                            this->mincanboostchance = 15;
                        }
                    }
                    if (this->type2boost) {
                        //if (!toad::clicker::r::right_higher_cps) { this->mincandropchance -= 0.5f; }
                        //else if (toad::clicker::r::right_higher_cps) { this->mincandropchance -= 0.1f; }

                        this->type2counterboost = 0;
                        this->min2 -= 1.4f;
                        this->max2 += 0.6f;
                        this->type2counterboost2++;
                    }
                    if (this->type2counterboost2 > 5) {
                        this->type2counterboost = 0;
                        this->type2boost = false;
                        this->type2counterboost2++;
                        if (type2counterboost2 > 45) {
                            this->min2 += 1.4f;
                            this->max2 -= 0.6f;
                            if (this->type2counterboost2 > 50) {
                                this->type2counterboost = 0;
                                this->type2counterboost2 = 0;
                                this->type2counter = 0;
                                this->type2counter2 = 0;
                            }
                        }
                    }
                    //drop
                    if (this->type2counter > toad::random_float(this->mincandropchance, 20)) {
                        if (!this->type2boost && !this->type2counterboost2 > 0) {
                            this->type2drop2 = true;
                            this->mincandropchance = 15;
                        }
                    }
                    if (type2drop2) {
                        //if (toad::clicker::r::right_higher_cps) { this->mincanboostchance -= 0.5f; }
                        //else if (!toad::clicker::r::right_r::right_higher_cps) { this->mincanboostchance -= 0.1f; }
                        this->type2counter = 0;
                        this->max2 += 1.6f;
                        this->type2counter2++;
                    }
                    if (this->type2counter2 > 5) {
                        this->type2counter = 0;
                        this->type2drop2 = false;
                        this->type2counter2++;
                        if (this->type2counter2 > 45) {
                            this->max2 -= 1.6f;
                            if (this->type2counter2 > 50) {
                                this->type2counter = 0;
                                this->type2counter2 = 0;
                                this->type2counterboost = 0;
                                this->type2counterboost2 = 0;
                            }
                        }
                    }
                }
            }
        }
    }   
}
