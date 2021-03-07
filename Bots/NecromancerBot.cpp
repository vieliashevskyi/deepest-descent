#include "../GameClasses.h"

NecromancerBot::NecromancerBot(int xx, int yy) throw() : Bot(xx, yy) {
	SetOwner(OwnerID_Necromancer);
	SetChar('N');
};

int	NecromancerBot::BotMove(Maze &ob) throw() {
	int _move = 4,
		bx = GetXCoord(),
		by = GetYCoord();

    /*
        Necromancer bot 1st super power is to revive another fallen bot
    */
	Revive(bx, by, ob);

    /*
        2nd super power is to drop Life item or Score Point
    */
	if(rand()%50 == 0) {
        ob.SetTile(bx, by, TileID_Life);
    } else {
        ob.SetTile(bx, by, TileID_ScorePoint);
    }

    /*
        Here be Dragons. Following code is non deliberately obfuscated. But it works. Game dev is messy at times :)
    */
	if (ob.sx == bx) {
        if ((by-1) == ob.sy) {
            return 0;
        }
        if ((by+1) == ob.sy) {
            return 2;
        }
    }	
	if (ob.sy == by) {
        if ((bx-1) == ob.sx) {
            return 3;
        }
        if ((bx+1) == ob.sx) {
            return 1;
        }
    }

    int nx = bx, ny = by,
		k = 0, wway = 4,
		rx, ry;
    bool bStaire = true, bRisky = false;

    STEP_1:
    if (nx < ob.sx) {
	    if (ob.Mat[nx+1][ny] != 0) {
            if (_move == 4) { 
                _move=1; 
            } 
            nx++;
            if (bRisky) {
                rx = nx;
                ry = ny;
            }
			if (ny == ob.sy) {
                goto STEP_1;
            } else if (ny < ob.sy) {
                if (ob.Mat[nx][ny+1] != 0) {
                    ny++;
                } else {
                    goto STEP_1;
                }
            } else if (ny > ob.sy) {
                if (ob.Mat[nx][ny-1] != 0) {
                    ny--;
                } else {
                    goto STEP_1;
                }
            }
		} else {
			STEP_2:
			if (ny < ob.sy) {
                if (ob.Mat[nx][ny+1] != 0) {
                    if (_move == 4) {
                        _move = 2;
                    }
                    ny++;
                } else {
                    if (!(_move == 2 || _move == 4)) {
                        nx = bx;
                        ny = by;
                        wway = _move;
                        _move = 4;
                        goto STEP_2;
                    } else {
                        if (_move == 4) {
                            if (wway != 4) {
                                return wway;
                            }
                        }
                        bStaire = false;
                        goto END;
                    }
                }
            } else if (ny > ob.sy) {
                if (ob.Mat[nx][ny-1] != 0) {
                    if (_move == 4) {
                        _move = 0;
                    }
                    ny--;
                } else {
                    if (!(_move == 0 || _move == 4)) {
                        nx = bx;
                        ny = by;
                        wway = _move;
                        _move = 4;
                        goto STEP_2;
                    } else {
                        if (_move == 4) {
                            if (wway != 4){
                                return wway;
                            }
                        }
                        bStaire = false;
                        goto END;
                    }
                }
            } else if (ny == ob.sy) {
				if (bRisky) {
                    if (!(ny == ry && nx == rx)) {
                        if (k > 5) {
                            return _move;
                        }
                        k++;
                        ny = ry;
                        nx = rx;
                        goto STEP_1;
                    } else {
                        bStaire = false;
                        goto END;
                    }
				}

                if (ny < by) {
					if ((ny-1) >= 0 && ob.Mat[bx][by-1] != 0) {
                        ny--;
                        bRisky = true;
                        rx = nx;
                        ry = ny;
						if (nx < ob.sx) {
                            nx++;
                        } else { 
                            nx--;
                        }
						
                        while (ob.Mat[nx][ny] == 0) {
                            if ((ny-1) >= 0) {
                                ny--;
                            } else { 
                                break;
                            }
                        }
						
                        if (ob.Mat[nx][ny] != 0) { 
                            goto STEP_1;
                        } else { 
                            return _move;
                        }
                    } else {
                        return _move;
                    }
                } else if (ny > by) {
                    if ((ny + 1) < ob.maxy && ob.Mat[bx][by+1] != 0) {
                        ny++;
                        bRisky = true;
                        rx = nx;
                        ry = ny;
						if (nx < ob.sx) {
                            nx++;
                        } else {
                            nx--;
                        }
						
                        while (ob.Mat[nx][ny] == 0) {
                            if ((ny + 1) < ob.maxy) {
                                ny++;
                            } else {
                                break;
                            }
                        }
						if (ob.Mat[nx][ny] != 0) {
                            goto STEP_1;
                        } else {
                            return _move;
                        }
                    } else { 
                        return _move;
                    }
                } else {
					if (ny == (ob.maxy-1)) {
                        if (_move == 4) {
                            if (ob.Mat[bx][by-1] != 0) {
                                return 0;
                            }
                            if (nx > ob.sx) {
                                return 3;
                            } else {
                                return 1;
                            }
                        }
                    } else if (ny == 0) {
                        if (_move == 4) {
                            if(ob.Mat[bx][by+1] != 0) {
                                return 2;
                            }
                            if (nx > ob.sx) {
                                return 3;
                            } else {
                                return 1;
                            }
                        }
                    } else if (ny >= ((int)(ob.maxy)/2)) {
                        if (_move == 4) {
                            if (ob.Mat[bx][by-1] != 0) {
                                return 0;
                            }
                            if (nx < ob.sx) {
                                return 3;
                            } else {
                                return 1;
                            }
                        }
                    } else {
                        if (_move == 4) {
                            if (ob.Mat[bx][by+1] != 0) {
                                return 2;
                            }
                            if (nx < ob.sx) {
                                return 3;
                            } else {
                                return 1;
                            }
                        }
                    }
				}
			}
		}

        if ((nx == ob.sx) && (ny == ob.sy)) {
            bStaire = true;
            goto END;
        } else {
            goto STEP_1;
        } 
    } else if (nx > ob.sx) {
		if (ob.Mat[nx-1][ny] != 0) {
            if (_move == 4) {
                _move=3;
            }
            nx--;	
			if (bRisky) {
                rx = nx;
                ry = ny;
            }
			if (ny == ob.sy) {
                goto STEP_1;
            } else if (ny < ob.sy) {
                if (ob.Mat[nx][ny+1] != 0) {
                    ny++;
                } else {
                    goto STEP_1;
                }
            } else if (ny > ob.sy) {
                if (ob.Mat[nx][ny-1] != 0) {
                    ny--;
                } else {
                    goto STEP_1;
                }
            }
		} else {
			goto STEP_2;
		}

        if ((nx == ob.sx) && (ny == ob.sy)) {
            bStaire = true;
            goto END;
        } else {
            goto STEP_1;
        }
    } else {
        if ((nx == ob.sx) && (ny == ob.sy)) {
            bStaire = true;
            goto END;
        } else {
            while (ny != ob.sy) { 
                if (ny > ob.sy) {
                    if (ob.Mat[nx][ny-1] != 0) {
                        ny--;
                        if (_move == 4) {
                            _move = 0;
                        }
                    } else {
                        break;
                    }
                }
				
                if (ny < ob.sy) {
                    if (ob.Mat[nx][ny+1] != 0) {
                        ny++;
                        if (_move == 4) {
                            _move = 2;
                        }
                    } else {
                        break;
                    }
                }
            }

			if (ny == ob.sy) {
                bStaire = true;
                goto END;
            }

			if (ny == by) {
                if (nx == (ob.maxx-1)) {
                    if (_move == 4) {
                        if (ob.Mat[nx-1][ny] != 0) {
                            return 3;
                        } else {
                            if (ny < ob.sy) { 
                                return 0;
                            } else {
                                return 2;
                            }
                        }
                        ny--;
                    }
                } else if (nx == 0) {
                    if(_move==4) {
                        if(ob.Mat[nx+1][ny]!=0) {
                            return 1;
                        } else {
                            if (ny < ob.sy) {
                                return 0;
                            } else {
                                return 2;
                            }
                        }
                        ny++;
                    }
                } else if (nx >= ((int)(ob.maxy)/2)) {
                    if (_move == 4) {
                        _move = 3;
                    }
                    bStaire = true;
                    goto END;
                } else {
                    if (_move == 4) {
                        _move = 1;
                    }
                    bStaire = true;
                    goto END;
                }
			} else {
				if (nx > (int)(ob.maxx/2)) {
                    nx++;
                    while (ob.Mat[nx][ny] == 0) {
                        if ((nx+1) < ob.maxx) {
                            nx++;
                        } else {
                            break;
                        }
                    }
					
                    if (ob.Mat[nx][ny] != 0 && nx < ob.maxx) {
                        goto STEP_2;
                    } else {
                        while (ob.Mat[nx][ny] == 0) {
                            if ((nx-1) >= 0) {
                                nx--;
                            } else {
                                break;
                            }
                        }
						
                        if (ob.Mat[nx][ny] != 0 && nx >= 0) {
                            goto STEP_2;
                        } else {
                            bStaire = false;
                            goto END;
                        }
					}
				} else {
                    nx--;
                    while (ob.Mat[nx][ny] == 0) {
                        if ((nx-1) >= 0) {
                            nx--;
                        } else {
                            break;
                        }
                    }
					
                    if (ob.Mat[nx][ny] != 0 && nx < ob.maxx) {
                        goto STEP_2;
                    } else {
                        while (ob.Mat[nx][ny] == 0) {
                            if ((nx+1) < ob.maxx) {
                                nx++;
                            } else {
                                break;
                            }
                        }
						
                        if (ob.Mat[nx][ny] != 0 && nx >= 0) {
                            goto STEP_2;
                        } else { 
                            bStaire = false;
                            goto END;
                        }
					}
				}
			}	    
		}
    }

    END:
    if (!bStaire) {
        if (_move == 1) {
            if ((bx-1) >= 0 && ob.Mat[bx-1][by] != 0) {
                return 3;
            } else { 
                return 1;
            }
        } else if (_move == 3) {
            if ((bx+1) < ob.maxx && ob.Mat[bx+1][by] != 0) {
                return 1; 
            } else {
                return 3;
            }
        } else if (_move == 0) {
            if ((by+1) < ob.maxy && ob.Mat[bx][by+1] != 0) {
                return 2; 
            } else {
                return 0;
            }
        } else if (_move == 2) {
            if ((by-1) >= 0 && ob.Mat[bx][by-1] != 0) {
                return 0;
            } else {
                return 2;
            }
        } else {
            goto LAST;
        }
    } else {
    	if (_move != 4) {
		    return _move;
        } else {
		    goto LAST;
        }
    }

LAST:
    _move = rand()%4;
    switch (_move) {
        case 0:	if((by-1) >= 0 && ob.Mat[bx][by-1] != 0) {
                    return 0;
                } else { 
                    goto LAST;
                }
                break;
        case 1:	if ((bx+1) < ob.maxx && ob.Mat[bx+1][by] != 0) {
                    return 1;
                } else {
                    goto LAST;
                }
                break;
        case 2:	if ((by+1) < ob.maxy && ob.Mat[bx][by+1] != 0) {
                    return 2;
                } else {
                    goto LAST;
                }
                break;
        case 3:	if ((bx-1) >= 0 && ob.Mat[bx-1][by] != 0) {
                    return 3;
                } else {
                    goto LAST;
                }
                break;
    }
    
    return _move;
};

void NecromancerBot::Revive(int xx, int yy, Maze &ob) throw() {
    Bot *tmp = ob.innerBot;
    while (tmp != NULL) {
        if (!tmp->IsAlive()) {
            if (tmp->GetXCoord() == xx && tmp->GetYCoord() == yy && tmp->GetOwner() != OwnerID_ArmedBomb) {
                tmp->alive = true;
            }
        }
        tmp = tmp->next;
    }
};