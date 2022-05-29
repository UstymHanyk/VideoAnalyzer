# Video analysis

### Prerequisites
- c++
- OpenCV
- TBB

### Color Analysis
To represent most prominent colors in a video we use a color spectrum and a graph. 
We chose to analyze colors in HSL color space. Unlike RGB it conveniently gives us hue, saturation and luminance(or lightness) - 3 qualities that are convenient and human-friendly. With HSL we could easily dissect the hues of the colors from their properties i.e. light blue, dark blue and blue have the same hue but different luminance and saturation, which helps us accomplish our mission, determine the promimence of different colors in a video. 

We have two properties to deal with -- saturation and lightness. With saturation it is simple, the more saturated the color the more eye-catchy it is. With lumimance it aslo quite easy, the most promiment colors are at the 50%, as with the luminance increase colors get more white, washed-out and with the decrease tehy get darker. These ideas are combined in a [habr post by akurilov](https://habr.com/ru/post/524978/), where he provides the formula for the "weight" of the color:
> W (S, L) = S * (0.52 - (0.5 - L)2) / 0.52

### Results
As a result we provide a color spectrum, where the graph represents the promimence of the colors, and the higher the hue the more saturated it is.

For example a graph of the Joker(2019) trailer represnts film's color scheme quite accurately
![result](test.png)



## Team
- [Bronytskyi Mykhailo]
- [Dutka Ostap](https://github.com/Ostap2003)
- [Hanyk Ustym](https://github.com/UstymHanyk)
- [Hentosh Petro](https://github.com/PHentosh)

