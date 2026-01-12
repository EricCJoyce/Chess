//////////////////////////////////////////////////////////////////////
//   R E N D E R I N G   (Things to do every instant)
function render()
  {
    if(elementsLoaded == ELEMENTS_TO_LOAD)                          //  If all textures are loaded
      {
        if(gameStarted && !gameOver)
          {
            var currentTime = Date.now();                           //  Update game clock
            var whiteClockDiv = document.getElementById('white-clock-div');
            var blackClockDiv = document.getElementById('black-clock-div');

            if(currentTime - previousSecond >= 1000)                //  In milliseconds
              {
                previousSecond = currentTime;                       //  Update the previous second
                                                                    //  If this is a clock game, we must update the clock...
                                                                    //  However, this front-end clock is for display only.
                if(gameStarted && clockgame && !gameOver)           //  A loss due to time will be caught and handled by the back-end.
                  {
                    if(CurrentTurn == 'White')
                      {
                        if(--whiteTimeSec < 0)
                          {
                                                                    //  Is White's time up?
                            if(whiteTimeMin <= 0 && whiteTimeSec <= 0)
                              whiteTimeSec = 0;                     //  Avoid having the clock say -1
                            else
                              {
                                whiteTimeMin--;
                                whiteTimeSec = 59;
                              }
                          }
                      }
                    else
                      {
                        if(--blackTimeSec < 0)
                          {
                                                                    //  Is Black's time up?
                            if(blackTimeMin <= 0 && blackTimeSec <= 0)
                              blackTimeSec = 0;                     //  Avoid having the clock say -1
                            else
                              {
                                blackTimeMin--;
                                blackTimeSec = 59;
                              }
                          }
                      }

                    whiteClockDiv.innerHTML = whiteTimeMin + ':' + ("0" + whiteTimeSec).slice(-2);
                    blackClockDiv.innerHTML = blackTimeMin + ':' + ("0" + blackTimeSec).slice(-2);
                  }
              }

            if(!animating)                                          //  Unless the state is in transition...
              {
                //  Pulse the Player.
                //  Check values from the Player.
              }
          }

        TWEEN.update();                                             //  Update any/all Tweens

        requestAnimationFrame(render);
        renderer.render(scene, camera);                             //  Draw
      }
  }