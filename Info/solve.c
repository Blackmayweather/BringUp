// the basic algorithme for solving a variant of the problem.

int con(int T[7][7],int S[6][6],int i,int j){
    int cmpt=0;
    if(S[i-1][j-1]==2)cmpt++;
    if(S[i-1][j+1]==1)cmpt++;
    if(S[i+1][j-1]==1)cmpt++;
    if(S[i+1][j+1]==2)cmpt++;
    return cmpt;
}
void fill(int T[7][7], int S[6][6],int i,int j, int type){
    switch(type){
        case 1:
            if(S[i-1][j-1]==0){
                    S[i-1][j-1]=(T[i-1][j-1]>con(T,S,i-1,j-1))?1:S[i-1][j-1];
            }else{
                if(S[i-1][j]==0){
                    S[i-1][j]=(T[i-1][j+1]>con(T,S,i-1,j+1))?1:S[i-1][j];
                }else{
                    if(S[i][j-1]==0){
                            S[i][j-1]=(T[i+1][j-1]>con(T,S,i+1,j-1))?1:S[i][j-1];
                    }else{
                        if(S[i][j]==0)S[i][j]=(T[i+1][j+1]>con(T,S,i+1,j+1))?2:S[i][j];
                    }
                }
            }
        break;
        case 2:
            if(S[i-1][j-1]==0){
                    S[i-1][j-1]=(T[i-1][j-1]>con(T,S,i-1,j-1))?1:S[i-1][j-1];
                    fill(T,S,i,j,1);
            }else{
                if(S[i-1][j]==0){
                    S[i-1][j]=(T[i-1][j+1]>con(T,S,i-1,j+1))?1:S[i-1][j];
                    fill(T,S,i,j,1);
                }else{
                    if(S[i][j-1]==0){
                            S[i][j-1]=(T[i+1][j-1]>con(T,S,i+1,j-1))?1:S[i][j-1];
                            fill(T,S,i,j,1);
                    }else{
                        if(S[i][j]==0)S[i][j]=(T[i+1][j+1]>con(T,S,i+1,j+1))?2:S[i][j];
                        fill(T,S,i,j,1);
                    }
                }
            }
        break;
        case 3:
            if(S[i-1][j-1]==0){
                    S[i-1][j-1]=(T[i-1][j-1]>con(T,S,i-1,j-1))?1:S[i-1][j-1];
                    fill(T,S,i,j,2);
            }else{
                if(S[i-1][j]==0){
                    S[i-1][j]=(T[i-1][j+1]>con(T,S,i-1,j+1))?1:S[i-1][j];
                    fill(T,S,i,j,2);
                }else{
                    if(S[i][j-1]==0){
                            S[i][j-1]=(T[i+1][j-1]>con(T,S,i+1,j-1))?1:S[i][j-1];
                            fill(T,S,i,j,2);
                    }else{
                        if(S[i][j]==0)S[i][j]=(T[i+1][j+1]>con(T,S,i+1,j+1))?2:S[i][j];
                        fill(T,S,i,j,2);
                    }
                }
            }
        break;
    }
}
void solve(int T[7][7],int S[6][6]){
    int i,j;
    //les 4..
    for(i=0;i<7;i++){
        for(j=0;j<7;j++){
            if(T[i][j]==4){
                S[i-1][j-1]=(T[i-1][j-1]>con(T,S,i-1,j-1))?2:S[i-1][j-1];
                S[i-1][j]=(T[i-1][j+1]>con(T,S,i-1,j+1))?1:S[i-1][j];
                S[i][j-1]=(T[i+1][j-1]>con(T,S,i+1,j-1))?1:S[i][j-1];
                S[i][j]=(T[i+1][j+1]>con(T,S,i+1,j+1))?2:S[i][j];
            }
        }
    }
    //les T[0][]
    i=0;for(j=0;j<7;j++){
        switch(T[i][j]){
            case 1:
                if(con(T,S,i,j)!=1){
                    if(con(T,S,i,j)==0){
                        if(S[i][j-1]==0){
                            S[i][j-1]=(T[i+1][j-1]>con(T,S,i+1,j-1))?1:S[i][j-1];
                        }else{
                            if(S[i][j]==0)S[i][j]=(T[i+1][j+1]>con(T,S,i+1,j+1))?2:S[i][j];
                        }
                    }
                }
            break;
            case 2:
                if(con(T,S,i,j)!=2){
                    switch(con(T,S,i,j)){
                        case 0:
                            S[i][j-1]=(T[i+1][j-1]>con(T,S,i+1,j-1))?1:S[i][j-1];
                            S[i][j]=(T[i+1][j+1]>con(T,S,i+1,j+1))?2:S[i][j];
                        break;
                        case 1:
                            if(S[i][j-1]==0){
                                S[i][j-1]=(T[i+1][j-1]>con(T,S,i+1,j-1))?1:S[i][j-1];
                            }else{
                                if(S[i][j]==0)S[i][j]=(T[i+1][j+1]>con(T,S,i+1,j+1))?2:S[i][j];
                            }
                        break;
                    }
                }
            break;
        }
    }
    //les T[6][]
    i=6;for(j=0;j<7;j++){
        switch(T[i][j]){
            case 1:
                if(con(T,S,i,j)!=1){
                    if(con(T,S,i,j)==0){
                        if(S[i-1][j-1]==0){
                            S[i-1][j-1]=(T[i-1][j-1]>con(T,S,i-1,j-1))?1:S[i-1][j-1];
                        }else{
                            if(S[i-1][j]==0)S[i-1][j]=(T[i-1][j+1]>con(T,S,i-1,j+1))?1:S[i-1][j];
                        }
                    }
                }
            break;
            case 2:
                if(con(T,S,i,j)!=2){
                    switch(con(T,S,i,j)){
                        case 0:
                            S[i-1][j-1]=(T[i-1][j-1]>con(T,S,i-1,j-1))?1:S[i-1][j-1];
                            S[i-1][j]=(T[i-1][j+1]>con(T,S,i-1,j+1))?1:S[i-1][j];
                        break;
                        case 1:
                            if(S[i-1][j-1]==0){
                                S[i-1][j-1]=(T[i-1][j-1]>con(T,S,i-1,j-1))?1:S[i-1][j-1];
                            }else{
                                if(S[i-1][j]==0)S[i-1][j]=(T[i-1][j+1]>con(T,S,i-1,j+1))?1:S[i-1][j];
                            }
                        break;
                    }
                }
            break;
        }
    }
    //les T[][0]
    j=0;for(i=1;i<6;i++){
        switch(T[i][j]){
            case 1:
                if(con(T,S,i,j)!=1){
                    if(con(T,S,i,j)==0){
                        if(S[i-1][j-1]==0){
                            S[i-1][j-1]=(T[i-1][j-1]>con(T,S,i-1,j-1))?1:S[i-1][j-1];
                        }else{
                            if(S[i][j-1]==0)S[i][j-1]=(T[i+1][j-1]>con(T,S,i+1,j-1))?1:S[i][j-1];
                        }
                    }
                }
            break;
            case 2:
                if(con(T,S,i,j)!=2){
                    switch(con(T,S,i,j)){
                        case 0:
                            S[i-1][j-1]=(T[i-1][j-1]>con(T,S,i-1,j-1))?1:S[i-1][j-1];
                            S[i][j-1]=(T[i+1][j-1]>con(T,S,i+1,j-1))?1:S[i][j-1];
                        break;
                        case 1:
                            if(S[i-1][j-1]==0){
                                S[i-1][j-1]=(T[i-1][j-1]>con(T,S,i-1,j-1))?1:S[i-1][j-1];
                            }else{
                                if(S[i][j-1]==0)S[i][j-1]=(T[i+1][j-1]>con(T,S,i+1,j-1))?1:S[i][j-1];
                            }
                        break;
                    }
                }
            break;
        }
    }
    //les T[][6]
    j=6;for(i=1;i<6;i++){
        switch(T[i][j]){
            case 1:
                if(con(T,S,i,j)!=1){
                    if(con(T,S,i,j)==0){
                        if(S[i-1][j-1]==0){
                            S[i-1][j-1]=(T[i-1][j-1]>con(T,S,i-1,j-1))?1:S[i-1][j-1];
                        }else{
                            if(S[i][j-1]==0)S[i][j-1]=(T[i+1][j-1]>con(T,S,i+1,j-1))?1:S[i][j-1];
                        }
                    }
                }
            break;
            case 2:
                if(con(T,S,i,j)!=2){
                    switch(con(T,S,i,j)){
                        case 0:
                            S[i-1][j-1]=(T[i-1][j-1]>con(T,S,i-1,j-1))?1:S[i-1][j-1];
                            S[i][j-1]=(T[i+1][j-1]>con(T,S,i+1,j-1))?1:S[i][j-1];
                        break;
                        case 1:
                            if(S[i-1][j-1]==0){
                                S[i-1][j-1]=(T[i-1][j-1]>con(T,S,i-1,j-1))?1:S[i-1][j-1];
                            }else{
                                if(S[i][j-1]==0)S[i][j-1]=(T[i+1][j-1]>con(T,S,i+1,j-1))?1:S[i][j-1];
                            }
                        break;
                    }
                }
            break;
        }
    }
    //les T[1-6][1-6]
    for(i=1;i<6;i++){
        for(j=1;j<6;j++){
            switch(T[i][j]){
                case 1:
                    if(con(T,S,i,j)!=1)fill(T,S,i,j,1);
                break;
                case 2:
                    if(con(T,S,i,j)!=2){
                        switch(con(T,S,i,j)){
                            case 0:
                                fill(T,S,i,j,2);
                            break;
                            case 1:
                                fill(T,S,i,j,1);
                            break;
                        }
                    }
                break;
                case 3:
                    if(con(T,S,i,j)!=3){
                        switch(con(T,S,i,j)){
                            case 0:
                                fill(T,S,i,j,3);
                            break;
                            case 1:
                                fill(T,S,i,j,2);
                            break;
                            case 2:
                                fill(T,S,i,j,1);
                            break;
                        }
                    }
                break;
            }
        }
    }
}