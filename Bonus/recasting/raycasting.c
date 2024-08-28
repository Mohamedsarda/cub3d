#include "../../recasting.h"

int has_wall(t_cub *cube, double x1, double y1, int is)
{
    if (is == 1)
        y1--;
    else if (is == 2)
        x1--;
    int x = floor(x1 / tile_size);
    int y = floor(y1 / tile_size);

    if (cube->data->map[y][x] == '1' || cube->data->map[y][x] == 'D' || cube->data->map[y][x] == 'O')
        return 1;
    return 0;
}

void ft_draw_hero(t_cub *cube, t_vars *vars)
{
    vars->y_intercept = floor(cube->player->player_y / tile_size) * tile_size;
    if (vars->israyfacingdown)
        vars->y_intercept += tile_size;

    vars->x_intercept = cube->player->player_x + (vars->y_intercept - cube->player->player_y) / tan(vars->angle);

    vars->ystep = tile_size;
    if (vars->israyfacingup)
        vars->ystep *= -1;

    vars->xstep = tile_size / tan(vars->angle);
    if (vars->israyfacingleft && vars->xstep > 0)
        vars->xstep *= -1;
    if (vars->israyfacingright && vars->xstep < 0)
        vars->xstep *= -1;

    vars->nexthorztouchx = vars->x_intercept;
    vars->nexthorztouchy = vars->y_intercept;

    int a = 0;
    if (vars->israyfacingup)
        a = 1;

    while (vars->nexthorztouchx > 0 && vars->nexthorztouchx < cube->data->map_cols * tile_size &&
           vars->nexthorztouchy > 0 && vars->nexthorztouchy < cube->data->map_row * tile_size)
           {
        int wall = has_wall(cube, vars->nexthorztouchx, vars->nexthorztouchy, a);
        if (wall == 1)
        {
            vars->foundhorzwallhit = 1;
            vars->horzwallhitx = vars->nexthorztouchx;
            vars->horzwallhity = vars->nexthorztouchy;
            vars->door = 0;
            break;
        }
        else
        {
            vars->nexthorztouchx += vars->xstep;
            vars->nexthorztouchy += vars->ystep;
        }
    }
}

void ft_draw_ver(t_cub *cube, t_vars *vars)
{
    vars->foundvertwallhit = 0;
    vars->vertwallhitx = 0;
    vars->vertwallhity = 0;

    vars->x_intercept = floor(cube->player->player_x / tile_size) * tile_size;
    if (vars->israyfacingright)
        vars->x_intercept += tile_size;

    vars->y_intercept = cube->player->player_y + (vars->x_intercept - cube->player->player_x) * tan(vars->angle);

    vars->xstep = tile_size;
    if (vars->israyfacingleft)
        vars->xstep *= -1;

    vars->ystep = tile_size * tan(vars->angle);
    if (vars->israyfacingup && vars->ystep > 0)
        vars->ystep *= -1;
    if (vars->israyfacingdown && vars->ystep < 0)
        vars->ystep *= -1;

    vars->nextverttouchx = vars->x_intercept;
    vars->nextverttouchy = vars->y_intercept;

    int a = 0;
    if (vars->israyfacingleft)
        a = 2;

    while (vars->nextverttouchx > 0 && vars->nextverttouchx < cube->data->map_cols * tile_size &&
           vars->nextverttouchy > 0 && vars->nextverttouchy < cube->data->map_row * tile_size)
           {
        int wall = has_wall(cube, vars->nextverttouchx, vars->nextverttouchy, a);
        if (wall == 1)
        {
            vars->foundvertwallhit = 1;
            vars->vertwallhitx = vars->nextverttouchx;
            vars->vertwallhity = vars->nextverttouchy;
            vars->door = 0;
            break;
        }
        else
        {
            vars->nextverttouchx += vars->xstep;
            vars->nextverttouchy += vars->ystep;
        }
    }
}

double distanceBetweenPoints(double x1,double y1,double x2,double y2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void    ft_get_close_wall(t_cub *cube, t_vars *vars)
{
    vars->horzhitdistance = __INT_MAX__;
    vars->verthitdistance = __INT_MAX__;
    if (vars->foundhorzwallhit)
        vars->horzhitdistance = distanceBetweenPoints(cube->player->player_x, cube->player->player_y, vars->horzwallhitx, vars->horzwallhity);
    if (vars->foundvertwallhit)
        vars->verthitdistance = distanceBetweenPoints(cube->player->player_x, cube->player->player_y, vars->vertwallhitx, vars->vertwallhity);
    vars->wallhitx = vars->vertwallhitx;
    vars->wallhity = vars->vertwallhity;
    vars->distance = vars->verthitdistance;
    if (vars->verthitdistance < vars->horzhitdistance)
    {
        vars->wallhitx = vars->vertwallhitx;
        vars->wallhity = vars->vertwallhity;
        vars->distance = vars->verthitdistance;
        vars->washitvert = 1;
    }
    else
    {
        vars->wallhitx = vars->horzwallhitx;
        vars->wallhity = vars->horzwallhity;
        vars->distance = vars->horzhitdistance;
        vars->washitvert = 0;
    }
}

t_vars draw_line(t_cub *cube, double angle, int is)
{
    (void)is;
    t_vars vars;
    vars.angle = normalizeAngle(angle);

    vars.israyfacingdown = vars.angle > 0 && vars.angle < M_PI;
    vars.israyfacingup = !vars.israyfacingdown;
    vars.israyfacingright = vars.angle < 0.5 * M_PI || vars.angle > 1.5 * M_PI;
    vars.israyfacingleft = !vars.israyfacingright;

    vars.foundhorzwallhit = 0;
    vars.horzwallhitx = 0;
    vars.horzwallhity = 0;
    vars.door = 0;

    ft_draw_hero(cube, &vars);
    ft_draw_ver(cube, &vars);
    ft_get_close_wall(cube, &vars);

    double fx = vars.wallhitx;
    double fy = vars.wallhity;
    if (vars.israyfacingleft)
        fx--;
    if (vars.israyfacingup)
        fy--;
    int x = floor(fx / tile_size);
    int y = floor(fy / tile_size);
    if (cube->data->map[y][x] == 'D')
        vars.door = 1;
    return vars;
}

void draw_lines(t_cub *cube, int is)
{
    cube->is = MAP_SCALE;
    double angle = cube->player->rotat_angle + (FOV_ANGLE / 2);
    int i = 0;
    while (i < NUM_RAYS)
    {
        draw_line(cube, angle, is);
        angle -= FOV_ANGLE / NUM_RAYS;
        i++;
    }
}
