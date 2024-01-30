
$fn = 256;

linear_extrude(0.0001)
translate([0.5, 0.5])
difference()
{
    circle(d = 0.81);
    circle(d = 0.8);
}
