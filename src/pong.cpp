#include "pong.h"

// window size and update rate (60 fps)
int width = 1000;
int height = 500;
int interval = 1000 / 60;

// Score
int score_left = 0;
int score_right = 0;

// Rackets in general
int racket_width = 10;
int racket_height = 80;
int racket_speed = 3;

// Left racket position
float racket_left_x = 10.0f;
float racket_left_y = 50.0f;

// Right racket position
float racket_right_x = width - racket_width - 10;
float racket_right_y = 50;

// Ball
float ball_pos_x = width/2;
float ball_pos_y = height/2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 8;
int ball_speed = 5;

void vec2_norm(float &x, float &y) {
    // Sets a vectors length to 1
    float length = std::sqrt((x*x) + (y*y));
    if (length != 0.0f) {
        length = 1.0f / length;
        x *= length;
        y *= length;
    }
}

void drawText(float x, float y, std::string text) {
    glRasterPos2f(x, y);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)text.c_str());
}

void drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
        glVertex2f(x            ,y         );
        glVertex2f(x + width    ,y         );
        glVertex2f(x + width    ,y + height);
        glVertex2f(x            ,y + height);
    glEnd();
}

void draw() {
    // clear (has to be done at the beginning)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Draw rackets
    drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
    drawRect(racket_right_x, racket_right_y, racket_width, racket_height);

    // Draw ball 
    drawRect(ball_pos_x - ball_size/2,
             ball_pos_y - ball_size/2,
             ball_size, ball_size);

    // Draw score
    std::string score_text = std::to_string(score_left) + ":" + 
                             std::to_string(score_right); 
    drawText(width/2 - 10, height - 15, score_text);

    // Swap buffers (has to be done at the end)
    glutSwapBuffers();
}

void keyboard() {
    // Left racket
    if (kb.getKeyState(KEY_W)) racket_left_y += racket_speed;
    if (kb.getKeyState(KEY_S)) racket_left_y -= racket_speed;

    // Right racket
    if (kb.getKeyState(KEY_UP)) racket_right_y += racket_speed;
    if (kb.getKeyState(KEY_DOWN)) racket_right_y -= racket_speed;

}

void updateBall() {
    // Fly a bit
    ball_pos_x += ball_dir_x*ball_speed;
    ball_pos_y += ball_dir_y*ball_speed;

    // Hit by left racket?
    if (ball_pos_x < racket_left_x + racket_width &&
        ball_pos_x > racket_left_x &&
        ball_pos_y < racket_left_y + racket_height &&
        ball_pos_y > racket_left_y) {
            float t = ((ball_pos_y - racket_left_y)/racket_height) - 0.5f;
            ball_dir_x = fabs(ball_dir_x); 
            ball_dir_y = t;
    }

    // Hit by right racket?
    if (ball_pos_x > racket_right_x && 
        ball_pos_x < racket_right_x + racket_width &&
        ball_pos_y < racket_right_y + racket_height &&
        ball_pos_y > racket_right_y) {
            float t = ((ball_pos_y - racket_right_y)/racket_height) - 0.5f;
            ball_dir_x = -fabs(ball_dir_x);
            ball_dir_y = t;
    }

    // Hit left wall?
    if (ball_pos_x < 0) {
        ++score_right;
        ball_pos_x = width / 2;
        ball_pos_y = height / 2;
        ball_dir_x = fabs(ball_dir_x);
        ball_dir_y = 0;
    }

    // Hit right wall?
    if (ball_pos_x > width) {
        ++score_left;
        ball_pos_x = width / 2;
        ball_pos_y = height / 2;
        ball_dir_x = -fabs(ball_dir_x);
        ball_dir_y = 0;
    }

    // Hit top wall?
    if (ball_pos_y > height) {
        ball_dir_y = -fabs(ball_dir_y);
    }

    // Hit bottom wall
    if (ball_pos_y < 0) {
        ball_dir_y = fabs(ball_dir_y);
    }

    // Make sure that the length of dir stays at 1
    vec2_norm(ball_dir_x, ball_dir_y);
}

void update(int value) {
    // Input handling
    keyboard();

    // Move ball
    updateBall();

    // Call update() againg in 'interval' miliseconds
    glutTimerFunc(interval, update, 0);

    // Redisplay frame
    glutPostRedisplay();
}

void enable2D(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// program entry point
int main(int argc, char** argv) {
    // initialize opengl (via glut)
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Pong");

    // Register callback functions
    glutDisplayFunc(draw);
    glutTimerFunc(interval, update, 0);

    // Setup scene to 2D mode and set draw color to white
    enable2D(width, height);
    glColor3f(1.0f, 1.0f, 1.0f);

    // start the whole thing
    glutMainLoop();
    return 0;
}
