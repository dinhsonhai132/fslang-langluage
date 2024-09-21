import random
import pygame
import math
def game():
    
    pygame.init()


    #GAME SYSTEM
    wight = 800
    height = 600
    size = (wight, height)
    screen = pygame.display.set_mode(size)
    pygame.display.set_caption("jumping_simulator")


    #COLOR
    RED = (255, 0, 0)
    BLUE = (0, 0, 255)
    WHITE = (255, 255, 255)
    PURPLE = (138, 43, 226)


    #FUNCTION
    def make_text(text, font_size, color, bool):
        font = pygame.font.Font(None, font_size)
        if bool == 0:
            Text = font.render(text, True, color)
            return Text
        else:
            return 0

    #CUBE VARIBLE
    cube_x = 0
    cube_y = 550
    cube_wight = 30
    cube_height = 30
    gravity = 0.5
    cube_speed = 2.25
    jump = False
    height = 375
    ground = True
    saitama = 800
    breath = False
    vel = 0
    jump_vel = 0
    jump_speed = 10

    #INSIDE GAME
    t = 0
    p = 0
    clock = pygame.time.Clock()

    #POINT
    point_x = random.randint(0, 500)
    point_y = 0
    point_speed = 7.5

    #LAVA
    lava_speed = 5
    lava_x = 800
    lava_y = 510


    #CODE
    run = True
    while run:
        clock.tick(65)
        screen.fill(WHITE)
        get_fps = str(clock.get_fps())
        fps = get_fps[:4]


        if float(get_fps) < 50 and float(get_fps) > 30:
            fps_check = make_text("fps {} normal".format(fps), 40, RED, 0)
        elif float(get_fps) < 30:
            fps_check = make_text("fps {} low".format(fps), 40, PURPLE, 0)
        else:
            fps_check = make_text("fps {} high".format(fps), 40, BLUE, 0)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False


        text = make_text("score: {}".format(p), 40, RED, 0)


        keys = pygame.key.get_pressed()


        if keys[pygame.K_a] or keys[pygame.K_LEFT]:
            cube_x -= cube_speed
            print("<player position at {}:x {}:y>".format(cube_x, cube_y))
        elif keys[pygame.K_d] or keys[pygame.K_RIGHT]:
            cube_x += cube_speed
            print("<player position at {}:x {}:y>".format(cube_x, cube_y))


        if keys[pygame.K_LSHIFT] and not breath:
            cube_speed = 7.5
            print("<player position at {}:x {}:y>".format(cube_x, cube_y))
            t += 2
        else:
            cube_speed = 2.25


        if t >= saitama:
            breath = True


        if breath:
            t -= 2


        if t <= 0:
            breath = False


        if keys[pygame.K_SPACE] and ground:
            jump = True
            print("<player position at {}:x {}:y>".format(cube_x, cube_y))
            jump_vel = jump_speed


        if jump:
            jump_vel -= gravity
            cube_y -= jump_vel
            ground = False
            if jump_vel <= 0:
                jump = False


        if not jump and not ground:
            vel += gravity
            cube_y += vel


        if cube_y >= 500:
            cube_y = 500
            ground = True
            vel = 0
            jump_vel = 0


        if cube_x <= 0:
            cube_x = 0
        elif cube_x >= wight - cube_wight:
            cube_x = wight - cube_wight


        point_y += point_speed
        lava_x -= lava_speed


        screen.blit(text, (0, 0))
        screen.blit(fps_check, (0, 40))


        pygame.draw.rect(screen, RED, (point_x, point_y, 10, 10))
        pygame.draw.rect(screen, BLUE, (t, 530, 8000, 5))
        pygame.draw.rect(screen, RED, (cube_x, cube_y, cube_wight, cube_height))
        pygame.draw.rect(screen, PURPLE, (lava_x, lava_y, 20, 20))


        point = pygame.Rect(point_x, point_y, 10, 10)
        player = pygame.Rect(cube_x, cube_y, cube_wight, cube_height)
        lava = pygame.Rect(lava_x, lava_y + 10, 20, 20)


        if pygame.Rect.colliderect(point, player):
            p += 5
            point_y = 0
            point_x = random.randint(0, 500)
            print("point: ", p)


        if point_y == 600:
            point_y = 0
            point_x = random.randint(0, 500)


        if pygame.Rect.colliderect(lava, player):
            p -= 1
            lava_x = 0


        if lava_x <= 0:
            lava_x = 800


        pygame.display.flip()

    pygame.quit()