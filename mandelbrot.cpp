#include <GL/glut.h>
#include <complex>

const double size = 2.0;
const int pixel = 720;
// プロット間隔
const double interval = 0.002;
const double n = 128;
const int g_WindowWidth = pixel;
const int g_WindowHeight = pixel;
const double magnification = 1.8;
std::complex<double> c_plot(0.0, 0.0);
std::complex<double> z_center(0.0, 0.0);
double scale = 1.0;

double x_get2DPos(int x) {
    double ce_x = 
    (double)x / (double)g_WindowWidth * 2.0 - 1.0;
    return ce_x;
}

double y_get2DPos(int y) {
    double ce_y = (double)y / (double)g_WindowHeight * 2.0 - 1.0;
    return ce_y * -1.0;
}

std::complex<double> getCenterComplex(int winx, int winy) {
    double ce_x = x_get2DPos(winx) * scale * size;
    double ce_y = y_get2DPos(winy) * scale * size;
    std::complex<double> ce(ce_x, ce_y);
    return ce;
}

// キーボードイベント処理
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
	case 'Q':
	case '\033':
		exit(0);  /* '\033' は ESC の ASCII コード */
	default:
		break;
	} 
	glutPostRedisplay();
}

// マウスイベント処理
void mouse(int button, int state, int x, int y) {
	if(state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
            z_center += getCenterComplex(x, y);
            scale /= magnification;
            break;
		break;
		case GLUT_MIDDLE_BUTTON:
		break;
		case GLUT_RIGHT_BUTTON:
            z_center += getCenterComplex(x, y);
            scale *= magnification;
            if (scale >= 1.0) {
                scale = 1.0;
                z_center.real(0.0);
                z_center.imag(0.0);
            }
    		break;
		default:
		break;
		}
		glutPostRedisplay(); // 再描画
	}
}

int check_c(std::complex<double> c, int n) {
    std::complex<double> zn(0.0, 0.0);
    int N = 0;
    
    while (true) {
        zn = zn * zn + c;
        N++;
        if (std::abs(zn) > 2.0 || N > n) {
            break;
        }
    }
    return N;
}

void draw_area() {
    for (double x = -1.0 + interval; x < 1.0; x += interval) {
        for (double y = -1.0 + interval; y < 1.0; y += interval) {
            c_plot.real(x * scale * size);
            c_plot.imag(y * scale * size);
            double d_plot = check_c(c_plot + z_center, n);

            // グレースケールで描画
            glColor3d(d_plot / (double)n, d_plot / (double)n,
                      d_plot / (double)n);
            
            glBegin(GL_QUADS); // 集合を点(微小な四角形)で描画
            glVertex2d(x , y);
            glVertex2d(x + interval, y);
            glVertex2d(x + interval, y + interval);
            glVertex2d(x, y + interval);
            glEnd();
        }
    }
}

void display() {
    std::complex<double> c_plot;
    glClearColor(0.2, 0.2, 1.0, 1.0);  // 消去色指定
    glClear(GL_COLOR_BUFFER_BIT);      // 画面消去
    draw_area();
    glFlush();  // 画面出力
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    glutCreateWindow("Manderbrot Sets");

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}