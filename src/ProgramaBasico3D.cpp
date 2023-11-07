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
Temporizador T;
double AccumDeltaT=0;


GLfloat AspectRatio, angulo=0;

// Controle do modo de projecao
// 0: Projecao Paralela Ortografica; 1: Projecao Perspectiva
// A funcao "PosicUser" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'p'
int ModoDeProjecao = 1;


// Controle do modo de projecao
// 0: Wireframe; 1: Faces preenchidas
// A funcao "Init" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'e'
int ModoDeExibicao = 1;

double nFrames=0;
double TempoTotal=0;
Ponto CantoEsquerdo = Ponto(-20,-1,-10);
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
        glRotatef(90, 1,0,0);

        glTranslated(CantoEsquerdo.x, CantoEsquerdo.z+25, CantoEsquerdo.y-12.5);
        for(int x=0; x<25;x++)
        {
            glPushMatrix();
            for(int y=0; y<15;y++)
            {
                DesenhaLadrilho(MediumGoldenrod, Brown);
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
Ponto target       = Ponto(-9, 0, 34.92);
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

    // third person view (from behind)
    gluLookAt(player.x, player.y+3, player.z+7,   // Posi��o do Observador
              target.x,target.y,target.z,     // Posi��o do Alvo
              0.0f,1.0f,0.0f);
    
    /*
    // top view
    gluLookAt(player.x, player.y+10, player.z,   // Posi��o do Observador
              target.x,target.y,target.z,     // Posi��o do Alvo
              0.0f,1.0f,0.0f);
    */

   /*
   // side view
    gluLookAt(player.x+5, player.y+1, player.z,   // Posi��o do Observador
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
        glutSolidCube(2);
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
        glScalef(0.5, 0.5, 1);
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
void TracaPontosDeControle(Ponto PC[])
{
    glPointSize(10);
    glBegin(GL_POINTS);
        glVertex3f(PC[0].x, PC[0].y, PC[0].z);
        glVertex3f(PC[1].x, PC[1].y, PC[1].z);
        glVertex3f(PC[2].x, PC[2].y, PC[2].z);
    glEnd();
}
// **************************************************************
Ponto CalculaBezier3(Ponto* PC, double t)
{
    Ponto P;
    double UmMenosT = 1-t;

    P =  PC[0] * UmMenosT * UmMenosT + PC[1] * 2 * UmMenosT * t + PC[2] * t*t;
    return P;
}

vector<array<Ponto,3>> trajetoriasDosTiros;
vector<double> parametrosTrajetoriasTiros;
void DesenhaTiros()
{
    // TODO: calcular colisao
    for (int i=0; i<trajetoriasDosTiros.size(); i++)
    {
        /*
        printf("Calculando o ponto atual da trajetoria (parametro %.2f):", parametrosTrajetoriasTiros[i]);
        Ponto* traj = trajetoriasDosTiros[i].data();
        traj[0].imprime("\tA = ");
        traj[1].imprime(" B = ");
        traj[2].imprime(" C = ");
        printf("\n");
        */

        if (parametrosTrajetoriasTiros[i] > 1.0) // o projetil ja terminou a trajetoria ao longo da curva
        {
            trajetoriasDosTiros.erase(trajetoriasDosTiros.begin()+i);
            parametrosTrajetoriasTiros.erase(parametrosTrajetoriasTiros.begin()+i);
        }
        else // o projetil ainda precisa se deslocar e nao terminou a trajetoria
        {
            Ponto localizacaoAtualDoTiro = CalculaBezier3(trajetoriasDosTiros[i].data(), parametrosTrajetoriasTiros[i]);
            // TODO: renderizar o projétil
            parametrosTrajetoriasTiros[i] += 0.02;
        }
    }
}

// **********************************************************************
//  void display( void )
// **********************************************************************
float currentRotationAngle = 0.0;
float currentCannonAngle = 0.0;
Ponto vetDirecaoCanhao = Ponto(1,0,0);
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
    glutSwapBuffers();
}

// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
const float FORCA_TIRO = 2;
void keyboard ( unsigned char key, int x, int y )
{
	switch ( key )
    {
        case 27:        // Termina o programa qdo
            exit ( 0 );   // a tecla ESC for pressionada
            break;
        case 'p':
            ModoDeProjecao = !ModoDeProjecao;
            glutPostRedisplay();
            break;
        case 'e':
            ModoDeExibicao = !ModoDeExibicao;
            init();
            glutPostRedisplay();
            break;
        case 'a': // mover a cabeca (alvo) para a esquerda
            currentRotationAngle += 1.5;
            obsTarVector.rotacionaY(1.5);
            vetDirecaoCanhao = Ponto(1,0,0);
            vetDirecaoCanhao.rotacionaZ(currentCannonAngle);
            vetDirecaoCanhao.rotacionaY(currentRotationAngle);
            target = player + obsTarVector;
            break;
        case 'd': // mover a cabeca (alvo) para a direita
            currentRotationAngle -= 1.5;
            obsTarVector.rotacionaY(-1.5);
            vetDirecaoCanhao = Ponto(1,0,0);
            vetDirecaoCanhao.rotacionaZ(currentCannonAngle);
            vetDirecaoCanhao.rotacionaY(currentRotationAngle);
            target = player + obsTarVector;
            break;
        case 'w': // andar para a frente
            player = player + obsTarVector;
            target = target + obsTarVector;
            break;
        case 's': // andar para tras
            player = player - obsTarVector;
            target = target - obsTarVector;
            break;
        case ' ': // disparar o tiro
            // TODO: arrumar o calculo dos pontos de controle da curva
            //printf("player/PosicaoVeiculo = (%.2f, %.2f, %.2f)\n", player.x, player.y, player.z);
            //printf("currentRotationAngle/AnguloVeiculo = %.2f\n", currentRotationAngle);
            //printf("currentCannonAngle/AnguloCanhao = %.2f\n", currentCannonAngle);
            //printf("vetDirecaoCanhao/DirecaoDoCanhao = (%.2f, %.2f, %.2f)\n", vetDirecaoCanhao.x, vetDirecaoCanhao.y, vetDirecaoCanhao.z);
            // calcular os pontos da Bezier
            Ponto posicaoDoCanhao = player + Ponto(CANNON_OFFSET_TO_BASE_X, CANNON_OFFSET_TO_BASE_Y, CANNON_OFFSET_TO_BASE_Z);
            Ponto ptoMaximoTrajetoria = posicaoDoCanhao + vetDirecaoCanhao * FORCA_TIRO;
            float distanciaInicioFim = 2*FORCA_TIRO*cos(currentCannonAngle*M_PI/180);
            Ponto finalTrajetoria = posicaoDoCanhao+Ponto(distanciaInicioFim, 0, 0);
            finalTrajetoria.rotacionaY(currentRotationAngle); // para alinhar com o veiculo
            
            array<Ponto,3> pontosBezier = {posicaoDoCanhao, ptoMaximoTrajetoria, finalTrajetoria};
            trajetoriasDosTiros.push_back(pontosBezier);
            parametrosTrajetoriasTiros.push_back(0.0);
            //printf("A = (%.2f, %.2f, %.2f)\n", posicaoDoCanhao.x, posicaoDoCanhao.y, posicaoDoCanhao.z);
            //printf("B = (%.2f, %.2f, %.2f)\n", ptoMaximoTrajetoria.x, ptoMaximoTrajetoria.y, ptoMaximoTrajetoria.z);
            //printf("C = (%.2f, %.2f, %.2f)\n", finalTrajetoria.x, finalTrajetoria.y, finalTrajetoria.z);
           //cout << "-----------------------------------------------------------------------------------------" << endl;
            break;
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
			if (currentCannonAngle+5 > 60) currentCannonAngle = 60;
            else                           currentCannonAngle += 5;
            vetDirecaoCanhao.rotacionaY(currentCannonAngle-prevCannonAngle);
			break;
	    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
			if (currentCannonAngle-5 < 0) currentCannonAngle = 0;
            else                          currentCannonAngle -= 5;
            vetDirecaoCanhao.rotacionaY(currentCannonAngle-prevCannonAngle);
			break;
		default:
			break;
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



