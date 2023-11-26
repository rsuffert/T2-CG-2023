// **********************************************************************
// PUCRS/Escola Polit�cnica
// COMPUTA��O GR�FICA
//
// Programa b�sico para criar aplicacoes 3D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <array>
#include "LeitorObjeto3D.h"
#include <ctime>
#include <cstdlib>

using namespace std;

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

#include "Temporizador.h"
#include "ListaDeCoresRGB.h"
#include "Ponto.h"
//#include <SOIL/SOIL.h>

#define N_AMIGOS_INIMIGOS 20

Objeto3D vacaModel;
struct Vaca {
    float posX, posY, posZ;
    bool inimigo, vivo;
};
Vaca* vacas;

Temporizador T;
double AccumDeltaT=0;
int pontos=0;

GLfloat AspectRatio, angulo=0;

// Controle do modo de projecao
// 0: Projecao Paralela Ortografica; 1: Projecao Perspectiva
// A funcao "PosicUser" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'p'
int ModoDeProjecao = 1;

bool ladrilhosMuro[25][15];

// Controle do modo de projecao
// 0: Wireframe; 1: Faces preenchidas
// A funcao "Init" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'e'
int ModoDeExibicao = 1;

double nFrames=0;
double TempoTotal=0;
Ponto CantoEsquerdo = Ponto(-20,-1,-10);
Ponto CantoEsquerdoParedao;
const float ANGULO_ROTACAO_PAREDAO = 90;
const float TRANSLACAO_PAREDAO_X = 0.0;
const float TRANSLACAO_PAREDAO_Y = 25.0;
const float TRANSLACAO_PAREDAO_Z = -12.5;
// **********************************************************************
//  void init(void)
//        Inicializa os parametros globais de OpenGL
// **********************************************************************
void init(void)
{
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // Fundo de tela preto

    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_CULL_FACE );
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);

    glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    if (ModoDeExibicao) // Faces Preenchidas??
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    srand(static_cast<unsigned int>(time(nullptr)));

    // inicializar objetos 3d
    vacaModel = Objeto3D();
    vacaModel.LeObjeto("Vaca.tri");
    vacas = new Vaca[N_AMIGOS_INIMIGOS];
    for (int i=0; i<N_AMIGOS_INIMIGOS; i++)
    {
        Vaca& v = vacas[i];
        v.posX = (CantoEsquerdo.x+2) + (rand() % 22);
        v.posY = 1.5;
        v.posZ = CantoEsquerdo.z + (rand() % 22);
        v.inimigo = i%2==0; // metade de amigos e metade de inimigos
        v.vivo = true;
    }

    // inicializar ladrilhos do muro
    for (int x=0; x<25; x++)
    {
        for (int y=0; y<15; y++)
        {
            ladrilhosMuro[x][y] = true;
        }
    }

    CantoEsquerdoParedao = Ponto(CantoEsquerdo.x+TRANSLACAO_PAREDAO_X, CantoEsquerdo.z+TRANSLACAO_PAREDAO_Y, CantoEsquerdo.y+TRANSLACAO_PAREDAO_Z);
    CantoEsquerdoParedao.rotacionaX(ANGULO_ROTACAO_PAREDAO);
}

// **********************************************************************
//
// **********************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualiza��o da tela em 30
    {
        AccumDeltaT = 0;
        angulo+= 1;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        //cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        //cout << "Nros de Frames sem desenho: " << nFrames << endl;
        //cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
}


// **********************************************************************
//  void DesenhaCubo()
// **********************************************************************
void DesenhaCubo(float tamAresta)
{
    glBegin ( GL_QUADS );
    // Front Face
    glNormal3f(0,0,1);
    glVertex3f(-tamAresta/2, -tamAresta/2,  tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2,  tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2,  tamAresta/2);
    glVertex3f(-tamAresta/2,  tamAresta/2,  tamAresta/2);
    // Back Face
    glNormal3f(0,0,-1);
    glVertex3f(-tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f(-tamAresta/2,  tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2, -tamAresta/2);
    // Top Face
    glNormal3f(0,1,0);
    glVertex3f(-tamAresta/2,  tamAresta/2, -tamAresta/2);
    glVertex3f(-tamAresta/2,  tamAresta/2,  tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2,  tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2, -tamAresta/2);
    // Bottom Face
    glNormal3f(0,-1,0);
    glVertex3f(-tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2,  tamAresta/2);
    glVertex3f(-tamAresta/2, -tamAresta/2,  tamAresta/2);
    // Right face
    glNormal3f(1,0,0);
    glVertex3f( tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2, -tamAresta/2);
    glVertex3f( tamAresta/2,  tamAresta/2,  tamAresta/2);
    glVertex3f( tamAresta/2, -tamAresta/2,  tamAresta/2);
    // Left Face
    glNormal3f(-1,0,0);
    glVertex3f(-tamAresta/2, -tamAresta/2, -tamAresta/2);
    glVertex3f(-tamAresta/2, -tamAresta/2,  tamAresta/2);
    glVertex3f(-tamAresta/2,  tamAresta/2,  tamAresta/2);
    glVertex3f(-tamAresta/2,  tamAresta/2, -tamAresta/2);
    glEnd();

}

Ponto CalculaPonto(Ponto p) {

    GLfloat ponto_novo[4];
    GLfloat matriz_gl[4][4];
    int  i;

    
    glGetFloatv(GL_MODELVIEW_MATRIX,&matriz_gl[0][0]);

    for(i=0; i<4; i++) {
        ponto_novo[i] = matriz_gl[0][i] * p.x +
                        matriz_gl[1][i] * p.y +
                        matriz_gl[2][i] * p.z +
                        matriz_gl[3][i];
    }
    Ponto out;
    out.x = ponto_novo[0];
    out.y = ponto_novo[1];
    out.z = -ponto_novo[2];

    return out;
}

// **********************************************************************
// void DesenhaLadrilho(int corBorda, int corDentro)
// Desenha uma c�lula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e est� sobre o plano XZ
// **********************************************************************
void DesenhaLadrilho(int corBorda, int corDentro)
{
    defineCor(corDentro);// desenha QUAD preenchido
    //glColor3f(1,1,1);
    glBegin ( GL_QUADS );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();

    defineCor(corBorda);
    glColor3f(0,1,0);

    glBegin ( GL_LINE_STRIP );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();
}

// **********************************************************************
//
//
// **********************************************************************
void DesenhaPiso()
{
    //srand(100); // usa uma semente fixa para gerar sempre as mesma cores no piso
    glPushMatrix();
    glTranslated(CantoEsquerdo.x, CantoEsquerdo.y, CantoEsquerdo.z);
    for(int x=0; x<25;x++)
    {
        glPushMatrix();
        for(int z=0; z<50;z++)
        {
            //DesenhaLadrilho(MediumGoldenrod, rand()%40);
            DesenhaLadrilho(Brown, LimeGreen);
            glTranslated(0, 0, 1);
        }
        glPopMatrix();
        glTranslated(1, 0, 0);
    }
    glPopMatrix();
}

void DesenhaParedao()
{
    // render the wall based on the floor
    glPushMatrix();
        glRotatef(ANGULO_ROTACAO_PAREDAO, 1,0,0);

        glTranslated(CantoEsquerdo.x+TRANSLACAO_PAREDAO_X, CantoEsquerdo.z+TRANSLACAO_PAREDAO_Y, CantoEsquerdo.y+TRANSLACAO_PAREDAO_Z);
        for(int x=0; x<25;x++)
        {
            glPushMatrix();
            for(int y=0; y<15;y++)
            {
                if (ladrilhosMuro[x][y]) DesenhaLadrilho(MediumGoldenrod, Brown); // desenha o ladrilho se ele nao tiver sido destruido
                glTranslated(0, 0, 1);
            }
            glPopMatrix();
            glTranslated(1, 0, 0);
        }
    glPopMatrix();
}

// **********************************************************************
//  void DefineLuz(void)
// **********************************************************************
void DefineLuz(void)
{
  // Define cores para um objeto dourado
  GLfloat LuzAmbiente[]   = {0.4, 0.4, 0.4 } ;
  GLfloat LuzDifusa[]   = {0.7, 0.7, 0.7};
  GLfloat LuzEspecular[] = {0.9f, 0.9f, 0.9 };
  GLfloat PosicaoLuz0[]  = {0.0f, 3.0f, 5.0f };  // Posi��o da Luz
  GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f};

   // ****************  Fonte de Luz 0

 glEnable ( GL_COLOR_MATERIAL );

   // Habilita o uso de ilumina��o
  glEnable(GL_LIGHTING);

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
  // Define os parametros da luz n�mero Zero
  glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa  );
  glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular  );
  glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0 );
  glEnable(GL_LIGHT0);

  // Ativa o "Color Tracking"
  glEnable(GL_COLOR_MATERIAL);

  // Define a reflectancia do material
  glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

  // Define a concentra��oo do brilho.
  // Quanto maior o valor do Segundo parametro, mais
  // concentrado ser� o brilho. (Valores v�lidos: de 0 a 128)
  glMateriali(GL_FRONT,GL_SHININESS,51);

}
// **********************************************************************

void MygluPerspective(float fieldOfView, float aspect, float zNear, float zFar )
{
    //https://stackoverflow.com/questions/2417697/gluperspective-was-removed-in-opengl-3-1-any-replacements/2417756#2417756
    // The following code is a fancy bit of math that is equivilant to calling:
    // gluPerspective( fieldOfView/2.0f, width/height , 0.1f, 255.0f )
    // We do it this way simply to avoid requiring glu.h
    //GLfloat zNear = 0.1f;
    //GLfloat zFar = 255.0f;
    //GLfloat aspect = float(width)/float(height);
    GLfloat fH = tan( float(fieldOfView / 360.0f * 3.14159f) ) * zNear;
    GLfloat fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

Ponto player       = Ponto(-9, 0, 35);
Ponto target       = Ponto(-9, 0, 34.94);
Ponto obsTarVector = target-player;
// **********************************************************************
//  void PosicUser()
// **********************************************************************
void PosicUser()
{
    // Define os par�metros da proje��o Perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define o volume de visualiza��o sempre a partir da posicao do
    // observador
    if (ModoDeProjecao == 0)
        glOrtho(-10, 10, -10, 10, 0, 7); // Projecao paralela Orthografica
    else MygluPerspective(90,AspectRatio,0.01,50); // Projecao perspectiva

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /*
    // third person view (from behind) following the player
    gluLookAt(player.x, player.y+3, player.z+7,   // Posi��o do Observador
              target.x,target.y,target.z,     // Posi��o do Alvo
              0.0f,1.0f,0.0f);
    */

   // third person (static)
    gluLookAt(
        -9, 3.5, 43,
        -9, 0, 34.92,
        0.0f, 1.0f, 0.0f
    );
    
    /*
    // top view
    gluLookAt(player.x, player.y+10, player.z,   // Posi��o do Observador
              target.x,target.y,target.z,     // Posi��o do Alvo
              0.0f,1.0f,0.0f);
    */

   /*
   // side view
    gluLookAt(player.x+50, player.y+1, player.z,   // Posi��o do Observador
              target.x,target.y,target.z,     // Posi��o do Alvo
              0.0f,1.0f,0.0f);
    */
}

// **********************************************************************
//  void reshape( int w, int h )
//		trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{

	// Evita divis�o por zero, no caso de uam janela com largura 0.
	if(h == 0) h = 1;
    // Ajusta a rela��o entre largura e altura para evitar distor��o na imagem.
    // Veja fun��o "PosicUser".
	AspectRatio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Seta a viewport para ocupar toda a janela
    glViewport(0, 0, w, h);
    //cout << "Largura" << w << endl;

	PosicUser();
}

void DesenhaParalelepipedo(float red, float green, float blue, float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, float rotationAngleX, float rotationAngleY, float rotationAngleZ)
{
    glPushMatrix();
        glColor3f(red, green, blue);
        glTranslatef(posX, posY, posZ);
        glRotatef(rotationAngleX, 1.0,0.0,0.0);
        glRotatef(rotationAngleY, 0.0,1.0,0.0);
        glRotatef(rotationAngleZ, 0.0,0.0,1.0);
        glScalef(scaleX, scaleY, scaleZ);
        glutSolidCube(1);
        //DesenhaCubo(1);
    glPopMatrix();
}

const float CANNON_OFFSET_TO_BASE_X = 0.0;
const float CANNON_OFFSET_TO_BASE_Y = 1.15;
const float CANNON_OFFSET_TO_BASE_Z = -0.7;
void DesenhaCanhao(float currentRotationAngle, float currentCannonAngle)
{
	glPushMatrix();
        glTranslatef(player.x, player.y, player.z);
        glRotatef(currentRotationAngle, 0.0,1.0,0.0);

        // desenha base do canhao
        glColor3f(0.678, 0.9, 0.902);
        glScalef(2, 1, 3);
        glutSolidCube(2);
        
        // desenha cano do canhao
        glColor3f(0.9412, 0.5725, 0.058);
        glTranslatef(CANNON_OFFSET_TO_BASE_X, CANNON_OFFSET_TO_BASE_Y, CANNON_OFFSET_TO_BASE_Z);
        glRotatef(currentCannonAngle, 1.0,0.0,0.0);
        glScalef(0.15, 0.15, 0.35);
        glutSolidCube(2);
    glPopMatrix();
}
// **************************************************************
Ponto CalculaBezier3(Ponto* PC, double t)
{
    Ponto P;
    double UmMenosT = 1-t;

    P =  PC[0] * UmMenosT * UmMenosT + PC[1] * 2 * UmMenosT * t + PC[2] * t*t;
    return P;
}

void DesenhaEsfera(Ponto p, float raio)
{
    glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(p.x, p.y, p.z);
        glutSolidSphere(raio, 50, 50);
    glPopMatrix();   
}

/*
 * Destroi os ladrilhos necessarios, dada a linha e a coluna do ladrilho com que o projetil colidiu.
 * Os comentarios consideram a seguinte configuracao dos ladrilhos, em que 'E' representa o ladrilho que foi atingido:
 *      A   B   C
 *      D   E   F
 *      G   H   I
 */
void DestruirLadrilhosParedao(int linhaLadrilhoCentral, int colunaLadrilhoCentral)
{
    ladrilhosMuro[colunaLadrilhoCentral][linhaLadrilhoCentral] = false; // destruir ladrilho E

    // destruir ladrilhos A, B e C
    if (linhaLadrilhoCentral > 0)
    {
        ladrilhosMuro[colunaLadrilhoCentral][linhaLadrilhoCentral-1] = false; // destruir ladrilho B
        if (colunaLadrilhoCentral > 0)    ladrilhosMuro[colunaLadrilhoCentral-1][linhaLadrilhoCentral-1] = false; // destruir ladrilho A
        if (colunaLadrilhoCentral < 25-1) ladrilhosMuro[colunaLadrilhoCentral+1][linhaLadrilhoCentral-1] = false; // destruir ladrilho C
    }

    // destruir ladrilhos G, H e I
    if (linhaLadrilhoCentral < 15-1)
    {
        ladrilhosMuro[colunaLadrilhoCentral][linhaLadrilhoCentral+1] = false; // destruir ladrilho H
        if (colunaLadrilhoCentral > 0)    ladrilhosMuro[colunaLadrilhoCentral-1][linhaLadrilhoCentral+1] = false; // destruir ladrilho G
        if (colunaLadrilhoCentral < 25-1) ladrilhosMuro[colunaLadrilhoCentral+1][linhaLadrilhoCentral+1] = false; // destruir ladrilho I
    }

    if (colunaLadrilhoCentral > 0)    ladrilhosMuro[colunaLadrilhoCentral-1][linhaLadrilhoCentral] = false; // destruir ladrilho D
    if (colunaLadrilhoCentral < 25-1) ladrilhosMuro[colunaLadrilhoCentral+1][linhaLadrilhoCentral] = false; // destruir ladrilho F
}

vector<bool> passouDoParedao;
/*
 * Calcula a colisao com o paredao e destroi os ladrilhos correspondentes, se houver colisao.
 * @param localizacaoAtualDoTiro o ponto que representa a localizacao atual do projetil no espaco.
 * @param idxTiro o indice do tiro.
 * @return true se houve colisao; false caso contrario.
 */
bool CalcularColisaoEDestruirParedao(Ponto localizacaoAtualDoTiro, int idxTiro)
{
    if (passouDoParedao[idxTiro]) return false;

    if (localizacaoAtualDoTiro.z <= CantoEsquerdoParedao.z) // colisao em Z
    {
        passouDoParedao[idxTiro] = true;
        if (localizacaoAtualDoTiro.x >= CantoEsquerdoParedao.x && localizacaoAtualDoTiro.x <= CantoEsquerdoParedao.x+25) // colisao em X
        {
            if (localizacaoAtualDoTiro.y <= CantoEsquerdoParedao.y && localizacaoAtualDoTiro.y >= CantoEsquerdoParedao.y-15) // colisao em Y
            {
                int linhaLadrilho = (int) (CantoEsquerdoParedao.y - localizacaoAtualDoTiro.y);
                int colunaLadrilho = (int) ((-1)*(CantoEsquerdoParedao.x - localizacaoAtualDoTiro.x));
                if (!ladrilhosMuro[colunaLadrilho][linhaLadrilho]) return false; // ladrilho ja foi destruido
                printf("Colisao com o muro, voce ganhou 5 pontos \n", linhaLadrilho, colunaLadrilho);
                pontos += 5;
                printf("Voce tem %d pontos \n\n", pontos);

                DestruirLadrilhosParedao(linhaLadrilho, colunaLadrilho);
                return true;
            }
        }
    }

    return false;
}


bool CalcularColisaoEDestruirVaca(Ponto localizacaoAtualDoTiro, int idxTiro)
{
    for (int i=0; i<N_AMIGOS_INIMIGOS; i++)
    {
        Vaca& v = vacas[i];
        if (!v.vivo) continue;// vaca ja foi destruida     
        
        Ponto ptVaca = (Ponto(v.posX, v.posY, v.posZ));
        if (Distancia(localizacaoAtualDoTiro, ptVaca) < 2.0)
        {
            if (v.inimigo == false) 
            {
                printf("Vaca amiga atingida, Voce perdeu 10 pontos \n");
                pontos -= 10;
                printf("Voce tem %d pontos\n\n", pontos);
            }    
            else
            {
                printf("Vaca inimiga atingida, Voce ganhou 10 pontos \n");
                pontos += 10;
                printf("Voce tem %d pontos \n\n", pontos);
            }  
            v.vivo = false;
            return true;
        }
    }
    return false;
}

vector<array<Ponto,3>> trajetoriasDosTiros;
vector<double> parametrosTrajetoriasTiros;
/*
 * Desenha os projeteis e os movimenta, alem de calcular colisoes.
 */
void DesenhaTiros()
{
    for (int i=0; i<trajetoriasDosTiros.size(); i++)
    {
        Ponto localizacaoAtualDoTiro = CalculaBezier3(trajetoriasDosTiros[i].data(), parametrosTrajetoriasTiros[i]);
        DesenhaEsfera(localizacaoAtualDoTiro, 0.5);

        if (localizacaoAtualDoTiro.y <= CantoEsquerdo.y) // colisao com o chao
        {
            printf("Voce perdeu 5 pontos porque atingiu o solo\n");
            pontos -= 5;
            printf("Voce tem %d pontos\n\n", pontos);

            // remover tiro
            trajetoriasDosTiros.erase(trajetoriasDosTiros.begin()+i);
            parametrosTrajetoriasTiros.erase(parametrosTrajetoriasTiros.begin()+i);
            passouDoParedao.erase(passouDoParedao.begin()+i);
        }

        if (parametrosTrajetoriasTiros[i] > 1.0) // o projetil ja terminou a trajetoria ao longo da curva e deve ser removido
        {
            trajetoriasDosTiros.erase(trajetoriasDosTiros.begin()+i);
            parametrosTrajetoriasTiros.erase(parametrosTrajetoriasTiros.begin()+i);
            passouDoParedao.erase(passouDoParedao.begin()+i);
        }
        else // o projetil ainda precisa se deslocar e nao terminou a trajetoria (calcular colisao e incrementar posicao)
        {
            bool colidiu = CalcularColisaoEDestruirParedao(localizacaoAtualDoTiro, i); // calcular colisao se o tiro ainda nao tiver passado do paredao
            bool coliVaca = CalcularColisaoEDestruirVaca(localizacaoAtualDoTiro, i);
            
            if (colidiu || coliVaca) // remover tiro
            {
                trajetoriasDosTiros.erase(trajetoriasDosTiros.begin()+i);
                parametrosTrajetoriasTiros.erase(parametrosTrajetoriasTiros.begin()+i);
                passouDoParedao.erase(passouDoParedao.begin()+i);
            }
            else parametrosTrajetoriasTiros[i] += 0.02;
            
        }
    }
}

Ponto posicaoDoCanhao;
Ponto ptoMaximoTrajetoria;
Ponto finalTrajetoria;
void DesenhaLinha(Ponto p1, Ponto p2) {
    glBegin(GL_LINES);
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
    glEnd();
}

// **********************************************************************
//  void display( void )
// **********************************************************************
float currentRotationAngle = 0.0;
float currentCannonAngle = 15.0;
Ponto vetDirecaoCanhao = Ponto(0, 0, 1);
float PosicaoZ = -30;
void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	DefineLuz();
	PosicUser();
	glMatrixMode(GL_MODELVIEW);
    DesenhaPiso();
    DesenhaParedao();
    DesenhaCanhao(currentRotationAngle, currentCannonAngle);
    DesenhaTiros();
    DesenhaLinha(posicaoDoCanhao, ptoMaximoTrajetoria);
    DesenhaLinha(ptoMaximoTrajetoria, finalTrajetoria);

    // exibir as vacas
    for (int i=0; i<N_AMIGOS_INIMIGOS; i++)
    {
        Vaca& v = vacas[i];
        //printf("Vaca(%.2f, %.2f, %.2f)\n", v.posX, v.posY, v.posZ);
        if (!v.vivo) continue;

        glPushMatrix();
            glTranslatef(v.posX, v.posY, v.posZ);
            glRotatef(-100, 1, 0, 0);
            glRotatef(60, 0, 0, 1);
            //glScalef(0.05, 0.05, 0.05);
            glScalef(0.07, 0.07, 0.07);
            if (v.inimigo) glColor3f(1.0f,0.0f,0.0f);
            else           glColor3f(0.0f, 0.0f, 1.0f);
            vacaModel.ExibeObjeto();
        glPopMatrix();
    }

    glutSwapBuffers();
}

// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
float forcaTiro = 4;
void keyboard ( unsigned char key, int x, int y )
{
    Ponto temp;
    float distanciaInicioFim;
	switch ( key )
    {
        case 27:// Termina o programa qdo
        {
            exit ( 0 );   // a tecla ESC for pressionada
            break;
        }
        case 'p':
        {
            ModoDeProjecao = !ModoDeProjecao;
            glutPostRedisplay();
            break;
        }
        case 'e':
        {
            ModoDeExibicao = !ModoDeExibicao;
            init();
            glutPostRedisplay();
            break;
        }
        case 'a': // mover a cabeca (alvo) para a esquerda
        {
            currentRotationAngle += 1.5;
            obsTarVector.rotacionaY(1.5);
            target = player + obsTarVector;
            break;
        }
        case 'd': // mover a cabeca (alvo) para a direita
        {
            currentRotationAngle -= 1.5;
            obsTarVector.rotacionaY(-1.5);
            target = player + obsTarVector;
            break;
        }
        case 'w': // andar para a frente
        {
            temp = player + obsTarVector;
            if ( (temp.z <= CantoEsquerdo.z+27) || (temp.z >= CantoEsquerdo.z+49.5) || (temp.x >= CantoEsquerdo.x+24) || (temp.x <= CantoEsquerdo.x) ) 
                return;
            player = temp;
            target = target + obsTarVector;
            break;
        }
        case 's': // andar para tras
        {
            temp = player - obsTarVector;
            if ( (temp.z <= CantoEsquerdo.z+27) || (temp.z >= CantoEsquerdo.z+49.5) || (temp.x >= CantoEsquerdo.x+24) || (temp.x <= CantoEsquerdo.x) ) 
                return;
            player = temp;
            target = target - obsTarVector;
            break;
        }      
        case ' ': // disparar o tiro
        {
            Ponto vetDirecaoCanhao = Ponto(0, 0, -1); // Vetor apontando para frente 
            vetDirecaoCanhao.rotacionaY(currentRotationAngle);
            vetDirecaoCanhao.rotacionaX(currentCannonAngle);  

            //normalizando o vetor
            float length = sqrt(vetDirecaoCanhao.x * vetDirecaoCanhao.x +
                                vetDirecaoCanhao.y * vetDirecaoCanhao.y +
                                vetDirecaoCanhao.z * vetDirecaoCanhao.z);
            // Evite dividir por zero
            if (length != 0.0) {
                vetDirecaoCanhao.x /= length;
                vetDirecaoCanhao.y /= length;
                vetDirecaoCanhao.z /= length;
            }

            float distanciaDoTiro =  forcaTiro; 

            Ponto posicaoDoCanhao = player + Ponto(CANNON_OFFSET_TO_BASE_X, CANNON_OFFSET_TO_BASE_Y, CANNON_OFFSET_TO_BASE_Z);

            Ponto ptoMaximoTrajetoria = posicaoDoCanhao + (vetDirecaoCanhao * distanciaDoTiro) * 0.6 * forcaTiro;

            Ponto finalTrajetoria =  ptoMaximoTrajetoria + (vetDirecaoCanhao * distanciaDoTiro) * forcaTiro ;
            finalTrajetoria.y = -10; //para que o tiro sempre desapareca no chao (-10)

            std::array<Ponto, 3> pontosBezier = {posicaoDoCanhao, ptoMaximoTrajetoria, finalTrajetoria};
            trajetoriasDosTiros.push_back(pontosBezier);
            parametrosTrajetoriasTiros.push_back(0.0);
            passouDoParedao.push_back(false);
            break;
        }
        case 'l': // aumentar forca do tiro
        {    
            if (forcaTiro+1 > 10) return;
            forcaTiro+=0.5;
            break;
        }
        case 'k': // reduzir forca do tiro
        {
            if (forcaTiro-1 < 3) return;
            forcaTiro-=0.5;
            break;
        }
    }
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
    float prevCannonAngle = currentCannonAngle;
	switch ( a_keys )
	{
		case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
        {
			if (currentCannonAngle+5 > 60) currentCannonAngle = 60;
            else                           currentCannonAngle += 5;
			break;
        }
        case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
		{
        	if (currentCannonAngle-5 < 15) currentCannonAngle = 15;
            else                          currentCannonAngle -= 5;
			break;
        }
        default:
        {
			break;
        }
	}
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main ( int argc, char** argv )
{
	glutInit            ( &argc, argv );
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
	glutInitWindowPosition (0,0);
	glutInitWindowSize  ( 700, 700 );
	glutCreateWindow    ( "Computacao Grafica - Exemplo Basico 3D" );

	init ();
    //system("pwd");

	glutDisplayFunc ( display );
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( arrow_keys );
	glutIdleFunc ( animate );

	glutMainLoop ( );
	return 0;
}



