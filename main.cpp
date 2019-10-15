/// Operations ToolKit: JPG/PNG/JPEG/BMP -> void/JPG
//  Created by Henry Bernardo Kochenborger de Avila.
#include <string>
#include <iostream>
#include <algorithm>
#include <TGUI/TGUI.hpp>

#define SCALESNUMBER 256
#define SIZEBUTTONX 300
#define SIZEX 256
#define SIZEY 256

#define RATIOSLIDERBUTTON 3/4
#define KERNELSIZE 3

#define NWIDGETS 22
#define COPY 0
#define SAVE 1
#define FLIPVERTICAL 2
#define FLIPHORIZONTAL 3
#define GREYSCALE 4
#define QUANTIZEBUTTON 5
//#define QUANTIZETEXT 6
#define HISTOGRAMBUTTON 6
#define BRIGHTNESSBUTTON 7
//#define BRIGHTNESSTEXT 9
#define CONTRASTBUTTON 8
#define CONTRASTTEXT 9
#define NEGATIVEBUTTON 10
#define EQUALIZEBUTTON 11
#define MATCHINGBUTTON 12
#define MATCHINGTEXT 13
#define ZOOMOUTTEXT 14
#define ZOOMOUTBUTTON 15
#define ZOOMINBUTTON 16
#define ROTATEBUTTON 17
#define CONVOLUTIONBUTTON 18
#define CONVOLUTIONCOMPONENTS 19

unsigned int closestNumber(unsigned int number, std::vector<unsigned int> vect)
{
    unsigned int i, closest = 0;


    for(i = 0; i < vect.size(); i++)
    {
        if(abs(int(vect[closest]) - number) > abs(int(vect[i]) - number))
            closest = i;
    }

    return closest;
}

class Image
{
    sf::Image img;
    sf::Texture texture;
    sf::Sprite sprite;

public:
    /// loadFromFile: std::string -> void
    //  Objective: receives a string which is the filename and load it
    //      to img, texture and sprite.
    void loadFromFile(std::string nof)
    {
        img.loadFromFile(nof);
        texture.loadFromImage(img);
        sprite.setTexture(texture);
    }

    /// getSprite: void -> sf::Sprite
    //  Objective: returns sprite (sf::Sprite) attribute.
    sf::Sprite getSprite()
    {
        return sprite;
    }

    /// getTexture: void -> sf::Texture
    //  Objective: returns texture (sf::Texture) attribute.
    sf::Texture getTexture()
    {
        return texture;
    }

    /// getImage: void -> sf::Image
    //  Objective: returns image (sf::Image) attribute.
    sf::Image getImage()
    {
        return img;
    }

    /// updateImage: sf::Image -> void
    //  Objective: given an sf::Image object, this function updates the referenced instance
    //      with this image.
    void updateImage(sf::Image image)
    {
        img = image;
        texture.loadFromImage(image);
        sprite.setTexture(texture, true);
    }

    /// copy: Image -> void
    //  Objective: given an Image object, this function attributes to the referenced instance.
    void copy(Image src)
    {
        Image::updateImage(src.getImage());
    }

    /// saveToFile: std::string -> void
    //  Objective: given a string, this function saves this instance in the same directory
    //      and the given string.
    void saveToFile(std::string filename)
    {
        img.saveToFile(filename);
    }

    /// getSize: void -> sf::Vector2u
    //  Objective: this function returns the image's size.
    sf::Vector2u getSize()
    {
        return img.getSize();
    }

    /// setPosition: float float -> void
    //  Objective: this function set the sprite's position (x,y) to the given parameters.
    void setPosition(float x, float y)
    {
        sprite.setPosition(x,y);
    }

    /// flipVertical: void -> void
    //  Objective: this function rotates the image in 180�.
    void flipVertical()
    {
        sf::Color pixel;
        sf::Vector2u sizeImg = img.getSize();

        for(unsigned int x=0; x < sizeImg.x; x++)
        {
            for(unsigned int y=0; y < sizeImg.y/2; y++)
            {
                pixel = img.getPixel(x, y);
                img.setPixel(x, y, img.getPixel(x, sizeImg.y-y-1));
                img.setPixel(x, sizeImg.y-y-1, pixel);
            }
        }
        updateImage(img);
    }

    /// flipHorizontal: void -> void
    //  Objective: this function does a horizontal flip on the image.
    //  Note: It works like a mirror.
    void flipHorizontal()
    {
        sf::Color pixel;
        sf::Vector2u sizeImg = img.getSize();

        for(unsigned int x=0; x < sizeImg.x/2; x++)
        {
            for(unsigned int y=0; y < sizeImg.y; y++)
            {
                pixel = img.getPixel(x, y);
                img.setPixel(x, y, img.getPixel(sizeImg.x-x-1, y));
                img.setPixel(sizeImg.x-x-1, y, pixel);
            }
        }
        updateImage(img);
    }

    /// toGreyScale: void -> void
    //  Objective: this function changes the image to gray scale using for all the three channels:
    //      Pixel = 0.299*RED + GREEN*0.587 + BLUE*0.114
    void toGreyScale()
    {
        sf::Color currentPixel;
        unsigned int color;
        sf::Vector2u sizeImg = img.getSize();

        for(unsigned int x=0; x < sizeImg.x; x++)
        {
            for(unsigned int y=0; y < sizeImg.y; y++)
            {
                currentPixel = img.getPixel(x,y);
                color = round(float(currentPixel.r)*0.299 + float(currentPixel.g)*0.587 + float(currentPixel.b)*0.114);
                img.setPixel(x,y, sf::Color(color, color, color));
            }
        }
        updateImage(img);
    }

    /// quantize: int -> void
    //  Objective: this function changes the image according the parameter. It defines how many
    //      shades of grey will be used. This is calculated based on rounding for floats to
    //      integers.
    void quantize(int scales)
    {
        sf::Color currentPixel;
        int color;
        sf::Vector2u sizeImg = img.getSize();

        for(unsigned int x=0; x < sizeImg.x; x++)
        {
            for(unsigned int y=0; y < sizeImg.y; y++)
            {
                currentPixel = img.getPixel(x,y);
                if(scales == 1)
                    color = 0;
                else
                    color = round(round(float(currentPixel.r)/255*float(scales-1))*255/float(scales-1));
                img.setPixel(x,y, sf::Color(color, color, color));
            }
        }
        updateImage(img);
    }

    /// histogram: void -> vector<unsigned int>
    //  Objective: this function measures how many pixels there are for each shade of grey, giving a SCALESNUMBER size array.
    std::vector<unsigned int> histogram()
    {
        std::vector<unsigned int> frequency;
        frequency.resize(SCALESNUMBER);

        for(unsigned int x = 0; x < getSize().x; x++)
        {
            for(unsigned int y = 0; y < getSize().y; y++)
            {
                frequency[img.getPixel(x,y).r]++;
            }
        }
        return frequency;
    }

    /// cumulativeHistogram: float -> vector<unsigned int>
    //  Objective: this function measures how many pixels there are between 0 and a shade of grey S, giving a SCALESNUMBER size array.
    std::vector<unsigned int> cumulativeHistogram(float norm)
    {
        std::vector<unsigned int> cumulativeFrequency;

        cumulativeFrequency = histogram();

        cumulativeFrequency[0] = std::round(cumulativeFrequency[0]*norm);
        for(unsigned int i = 1; i < SCALESNUMBER; i++)
        {
            cumulativeFrequency[i] = std::round(cumulativeFrequency[i]*norm);
            cumulativeFrequency[i] += cumulativeFrequency[i-1];
        }
        return cumulativeFrequency;
    }

    /// isGreyScaled: void -> Bool
    //  Objective: verifies if the image is in grey scale. If it is, returns True. Else, returns False.
    bool isGreyScaled()
    {
        sf::Color pixel;
        for(unsigned int x = 0; x < getSize().x; x++)
        {
            for(unsigned int y = 0; y < getSize().y; y++)
            {
                pixel = img.getPixel(x,y);
                if(pixel.r == pixel.g && pixel.r == pixel.b)
                    continue;
                else
                    return false;
            }
        }
        return true;
    }

    /// adjustBrightness: unint -> void
    //  Objective: given an unsigned int X which references to a number in [0, 255], this function
    //      adds X to each pixel in the image.
    void adjustBrightness(int bias)
    {
        std::cout << "ADJUST BRIGHTNESS " << bias << '\n';
        sf::Color pixel;
        for(unsigned int x = 0; x < getSize().x; x++)
        {
            for(unsigned int y = 0; y < getSize().y; y++)
            {
                pixel = img.getPixel(x,y);
                img.setPixel(x, y, sf::Color(std::max(std::min(int(pixel.r + bias),255), 0), std::max(std::min(int(pixel.g + bias), 255), 0), std::max(std::min(int(pixel.b + bias), 255),0)));
            }
        }
        updateImage(img);
    }

    /// adjustContrast: float -> void
    //  Objective: given a float X which references to a number in (0, 255], this function
    //      multiplies X to each pixel in the image.
    void adjustContrast(float gain)
    {
        sf::Color pixel;
        for(unsigned int x = 0; x < getSize().x; x++)
        {
            for(unsigned int y = 0; y < getSize().y; y++)
            {
                pixel = img.getPixel(x,y);
                img.setPixel(x, y, sf::Color(std::min((unsigned int)(pixel.r*gain),(unsigned int)255), std::min((unsigned int)(pixel.g*gain), (unsigned int)255), std::min((unsigned int)(pixel.b*gain), (unsigned int)255)));
            }
        }
        updateImage(img);
    }

    /// toNegative: void -> void
    //  Objective: set image to its negative (255 - px for each pixel px).
    void toNegative()
    {
        sf::Color pixel;
        for(unsigned int x = 0; x < getSize().x; x++)
        {
            for(unsigned int y = 0; y < getSize().y; y++)
            {
                pixel = img.getPixel(x,y);
                img.setPixel(x, y, sf::Color((unsigned int)(255 - pixel.r), (unsigned int)(255 - pixel.g), (unsigned int)(255 - pixel.b)));
            }
        }
        updateImage(img);
    }

    /// histogramEqualization: void -> void
    //  Objective: equalize the image's histogram.
    void histogramEqualization()
    {
        std::vector<unsigned int> hist;
        std::vector<float> hist_cum;
        hist = histogram();
        hist_cum.resize(hist.size());
        sf::Vector2u size = getSize();
        sf::Color pixel;
        Image grayImage;
        float alpha = float(SCALESNUMBER-1)/(size.x*size.y);

        grayImage.updateImage(img);
        grayImage.toGreyScale();
        hist = grayImage.histogram();
        hist_cum[0] = alpha*hist[0];
        for(unsigned int i = 1; i < hist.size(); i++)
            hist_cum[i] = hist_cum[i-1] + alpha*hist[i];

        for(unsigned int x = 0; x < size.x; x++)
        {
            for(unsigned int y = 0; y < size.y; y++)
            {
                pixel = img.getPixel(x,y);
                img.setPixel(x, y, sf::Color((unsigned int)hist_cum[pixel.r], (unsigned int)hist_cum[pixel.g], (unsigned int)hist_cum[pixel.b]));
            }
        }
        updateImage(img);
    }

    /// histogramMatching: Image -> void
    //  Objective: match this image's histogram with given image's histogram.
    void histogramMatching(Image target)
    {
        std::vector<unsigned int> hist_cum, hist_target_cum, HM;
        float alpha = float(target.getSize().x*target.getSize().y)/(getSize().x*getSize().y);

        hist_cum = cumulativeHistogram(alpha);

        hist_target_cum = target.cumulativeHistogram(1);

        for(unsigned int i = 0; i < SCALESNUMBER; i++)
            HM.push_back(closestNumber(hist_cum[i], hist_target_cum));

        for(unsigned int x = 0; x < getSize().x; x++)
        {
            for(unsigned int y = 0; y < getSize().y; y++)
            {
                img.setPixel(x, y, sf::Color((unsigned int)HM[img.getPixel(x, y).r], (unsigned int)HM[img.getPixel(x, y).r], (unsigned int)HM[img.getPixel(x, y).r]));
            }
        }
        updateImage(img);
    }

    /// zoomOut: (unsigned int) (unsigned int) -> void
    //  Objective: given a scale S, this function resizes the image by 1/S.
    void zoomOut(unsigned int sx, unsigned int sy)
    {
        float avgR, avgG, avgB;
        int pixels;
        sf::Image newImage;
        newImage.create(ceil(float(getSize().x)/sx), ceil(float(getSize().y)/sy), sf::Color(0, 0, 0));
        for(unsigned int i = 0; i < ceil(float(getSize().x)/sx); i++)
        {
            for(unsigned int j = 0; j < ceil(float(getSize().y)/sy); j++)
            {
                avgR = 0;
                avgG = 0;
                avgB = 0;
                pixels = 0;
                for(unsigned int x = 0; x < sx; x++)
                {
                    for(unsigned int y = 0; y < sy; y++)
                    {
                        if((x + i*sx) < getSize().x && (y + j*sy) < getSize().y)
                        {
                            avgR += float(img.getPixel(x+i*sx, y+j*sy).r);
                            avgG += float(img.getPixel(x+i*sx, y+j*sy).g);
                            avgB += float(img.getPixel(x+i*sx, y+j*sy).b);
                            pixels++;
                        }
                    }
                }
                avgR = avgR/pixels;
                avgG = avgG/pixels;
                avgB = avgB/pixels;
                newImage.setPixel(i, j, sf::Color(avgR, avgG, avgB));
            }
        }
        updateImage(newImage);
    }

    /// zoomIn: void -> void
    //  Objective: scales the image by 2 times.
    void zoomIn()
    {
        sf::Image newImage;
        sf::Color rightPx, bottomPx, diagPx;
        newImage.create(getSize().x*2, getSize().y*2, sf::Color(0, 0, 0));

        for(unsigned int x = 0; x < getSize().x; x++)
        {
            for(unsigned int y = 0; y < getSize().y; y++)
            {
                if(x != (getSize().x - 1) && y != (getSize().y - 1))
                {
                    rightPx = sf::Color(round(float(img.getPixel(x,y).r + img.getPixel(x, y+1).r)/2), round(float(img.getPixel(x,y).g + img.getPixel(x, y+1).g)/2), round(float(img.getPixel(x,y).b + img.getPixel(x, y+1).b)/2));
                    bottomPx = sf::Color(round(float(img.getPixel(x,y).r + img.getPixel(x+1, y).r)/2), round(float(img.getPixel(x,y).g + img.getPixel(x+1, y).g)/2), round(float(img.getPixel(x,y).b + img.getPixel(x+1, y).b)/2));
                    diagPx = sf::Color(round(float(img.getPixel(x,y).r + img.getPixel(x+1, y+1).r)/2), round(float(img.getPixel(x,y).g + img.getPixel(x+1, y+1).g)/2), round(float(img.getPixel(x,y).b + img.getPixel(x+1, y+1).b)/2));
                }
                else if(x == (getSize().x - 1) && y != (getSize().y - 1))
                {
                    rightPx = img.getPixel(x, y);
                    bottomPx = sf::Color(round(float(img.getPixel(x,y).r + img.getPixel(x+1, y).r)/2), round(float(img.getPixel(x,y).g + img.getPixel(x+1, y).g)/2), round(float(img.getPixel(x,y).b + img.getPixel(x+1, y).b)/2));
                    diagPx = sf::Color(round(float(img.getPixel(x,y).r + img.getPixel(x+1, y+1).r)/2), round(float(img.getPixel(x,y).g + img.getPixel(x+1, y+1).g)/2), round(float(img.getPixel(x,y).b + img.getPixel(x+1, y+1).b)/2));
                }
                else if(x != (getSize().x - 1) && y == (getSize().y - 1))
                {
                    rightPx = sf::Color(round(float(img.getPixel(x,y).r + img.getPixel(x, y+1).r)/2), round(float(img.getPixel(x,y).g + img.getPixel(x, y+1).g)/2), round(float(img.getPixel(x,y).b + img.getPixel(x, y+1).b)/2));
                    bottomPx = img.getPixel(x, y);
                    diagPx = sf::Color(round(float(img.getPixel(x,y).r + img.getPixel(x+1, y+1).r)/2), round(float(img.getPixel(x,y).g + img.getPixel(x+1, y+1).g)/2), round(float(img.getPixel(x,y).b + img.getPixel(x+1, y+1).b)/2));
                }
                else
                {
                    rightPx = img.getPixel(x, y);
                    bottomPx = img.getPixel(x, y);
                    diagPx = img.getPixel(x, y);
                }
                newImage.setPixel(x*2, y*2, img.getPixel(x,y));
                newImage.setPixel(x*2, y*2+1, rightPx);
                newImage.setPixel(x*2+1, y*2, bottomPx);
                newImage.setPixel(x*2+1, y*2+1, diagPx);
            }
        }
        updateImage(newImage);
    }

    /// rotateLeft: void -> void
    //  Objective: rotates the image 90º.
    void rotateLeft()
    {
        sf::Image newImage;
        newImage.create(img.getSize().y, img.getSize().x, sf::Color::Black);

        for(unsigned int x = 0; x < img.getSize().y; x++)
        {
            for(unsigned int y = 0; y < img.getSize().x; y++)
            {
                newImage.setPixel(x, img.getSize().x - y - 1, img.getPixel(y, x));
            }
        }
        updateImage(newImage);
    }

    /// rotateRight: void -> void
    //  Objective: rotates the image -90º.
    void rotateRight()
    {
        sf::Image newImage;
        newImage.create(img.getSize().y, img.getSize().x, sf::Color::Black);

        for(unsigned int x = 0; x < img.getSize().y; x++)
        {
            for(unsigned int y = 0; y < img.getSize().x; y++)
            {
                newImage.setPixel(img.getSize().y - x - 1, y, img.getPixel(y, x));
            }
        }
        updateImage(newImage);
    }

    /// convolution: std::vector<std::vector<float>>  -> void
    //  Objective: given a 3x3 kernel, this function applies convolution in the image with the kernel.
    void convolution(std::vector<std::vector<float> > kernel)
    {
        sf::Image newImage;
        float ac = 0;
        newImage.create(getSize().x, getSize().y, sf::Color::Black);

        std::cout << "Antes da convolucao\n";
        if(kernel.size() == 3 && kernel[0].size() == 3 && kernel[1].size() == 3 && kernel[2].size() == 3)
        {
            for(unsigned int x = 1; x < (getSize().x - 1); x++)
            {
                for(unsigned int y = 1; y < (getSize().y - 1); y++)
                {
                    ac = 0;
                    for(unsigned int i = 0; i < kernel.size(); i++)
                    {
                        for(unsigned int j = 0; j < kernel[0].size(); j++)
                        {
                            ac += (kernel[2 - i][2 - j])*(img.getPixel(x+i, y+j).r);
                        }
                    }
                    ac = std::max(std::min(ac, float(SCALESNUMBER-1)), float(0));
                    newImage.setPixel(x, y, sf::Color((unsigned int)ac, (unsigned int)ac, (unsigned int)ac));
                }
            }
        }
        std::cout << "Antes do update\n";
        updateImage(newImage);
        std::cout << "Depois da convolucao\n";
    }
};

std::vector<sf::RectangleShape> histogramSprites(std::vector<unsigned int> funct, int beginX)
{
    std::vector<sf::RectangleShape> columns;
    columns.resize(funct.size());
    unsigned int maxElement = *std::max_element(funct.begin(), funct.end());
    float scaleX = float(SIZEX)/funct.size();
    float scaleY = float(SIZEY)/maxElement;

    for(unsigned int i = 0; i < funct.size(); i++)
    {
        columns[i].setSize(sf::Vector2f(scaleX, funct[i]*scaleY));
        columns[i].setPosition(beginX + i*scaleX, SIZEY - funct[i]*scaleY);
        columns[i].setFillColor(sf::Color(0, i, i));
    }
    return columns;
}

void plotOnWindow(std::vector<sf::RectangleShape> histogram, sf::RenderWindow *window)
{
    for(unsigned int i = 0; i < histogram.size(); i++)
        window->draw(histogram[i]);
}

void plotHistogram(std::vector<unsigned int> funct, std::string name)
{
    sf::RenderWindow window(sf::VideoMode(SIZEX, SIZEY), name);
    sf::Event event;
    std::vector<sf::RectangleShape> columns = histogramSprites(funct, 0);

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        plotOnWindow(columns, &window);
        window.display();
    }
}

void showHistogram(Image *img, std::string name)
{
    plotHistogram(img->histogram(), name);
}

void showCumulativeHistogram(Image *img, std::string name)
{
    plotHistogram(img->cumulativeHistogram(1), name);
}

void showHistograms(std::vector<Image> images, std::string name)
{
    sf::Vector2u windowSize = sf::Vector2u(0, 0);
    std::vector<std::vector<sf::RectangleShape> > histograms;
    int beginX = 0;

    images[0].setPosition(0, SIZEY);
    for(unsigned int i = 0; i < images.size(); i++)
    {
        windowSize.x += images[i].getSize().x;
        windowSize.y = std::max(images[i].getSize().y, windowSize.y);
        histograms.push_back(histogramSprites(images[i].histogram(), beginX));

        if(i != 0)
            images[i].setPosition(beginX, SIZEY);

        beginX = windowSize.x;
    }

    windowSize.x = std::max(SIZEX*images.size(), windowSize.x);
    windowSize.y += SIZEY;

    sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), name);
    sf::Event event;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        for(unsigned int i = 0; i < histograms.size(); i++)
        {
            window.draw(images[i].getSprite());
            plotOnWindow(histograms[i], &window);
        }
        window.display();
    }
}

void equalizeAndPlot(Image *img)
{
    Image original;
    original.copy(*img);
    std::vector<Image> images;

    img->histogramEqualization();

    images.push_back(original);
    images.push_back(*img);

    showHistograms(images, "Histograms");

}

class SliderButton
{
private:
    tgui::Slider::Ptr slider;
    tgui::TextBox::Ptr text;
    tgui::Button::Ptr button;

public:
    void initialize(tgui::Gui *gui, sf::Vector2f limits, std::string name, void (Image::*ImageFunction)(int), Image *imgPtr, int *value)
    {
        slider = tgui::Slider::create(limits.x, limits.y);
        text = tgui::TextBox::create();
        button = tgui::Button::create(name);

        slider->connect("ValueChanged", &(this->updateValue), this, value);
        button->connect("pressed", ImageFunction, imgPtr, std::ref(*value));
        text->setReadOnly(true);

        gui->add(slider);
        gui->add(text);
        gui->add(button);
    }

    void setSize(sf::Vector2f size, float ratio)
    {
        slider->setSize(size.x, size.y/2);
        button->setSize(ratio*size.x, size.y/2);
        text->setSize((float(1)-ratio)*size.x, size.y/2);
    }

    sf::Vector2f getSize()
    {
        return sf::Vector2f(slider->getSize().x, 2*slider->getSize().y);
    }

    void setPosition(sf::Vector2f position)
    {
        slider->setPosition(position.x, position.y);
        button->setPosition(position.x, position.y + slider->getSize().y);
        text->setPosition(position.x + button->getSize().x, position.y + slider->getSize().y);
    }

    sf::Vector2f getPosition()
    {
        return slider->getPosition();
    }

    void updateValue(int *value)
    {
        *value = int(this->slider->getValue());
        text->setText(sf::String(std::to_string(*value)));
    }
};

void updateContrast(Image *img, tgui::EditBox::Ptr text, float *value)
{
    if(!(text->getText().isEmpty()))
    {
        *value = std::min(std::max(std::stof((text->getText()).toAnsiString()), float(0)), float(SCALESNUMBER-1));
        text->setText(sf::String(std::to_string(*value)));
        img->adjustContrast(*value);
    }
}

void updateMatching(Image *img, tgui::EditBox::Ptr srcImage)
{
    Image target;
    std::cout << srcImage->getText().toAnsiString();
    target.loadFromFile(srcImage->getText().toAnsiString());
    img->histogramMatching(target);
}

void updateZoomOut(Image *img, tgui::EditBox::Ptr sxText, tgui::EditBox::Ptr syText, Image original, sf::RenderWindow *window, int *beginX)
{
    unsigned int sx, sy;

    if(!(sxText->getText().isEmpty()) && !(syText->getText().isEmpty()))
    {
        sx = std::stoi(sxText->getText().toAnsiString());
        sy = std::stoi(syText->getText().toAnsiString());
        if(sx != 0 && sy != 0)
        {
            *beginX = original.getSize().x + floor(float(img->getSize().x)/sx);
            window->setSize(sf::Vector2u(*beginX + SIZEBUTTONX, std::max(original.getSize().y, (unsigned int)round(float(img->getSize().y)/sy))));
            img->zoomOut(sx, sy);
        }
    }
}

void updateZoomIn(Image *imgSrc, Image original, sf::RenderWindow *window, int *beginX)
{
    *beginX = original.getSize().x + imgSrc->getSize().x*2;
    window->setSize(sf::Vector2u(*beginX + SIZEBUTTONX, std::max(original.getSize().y, imgSrc->getSize().y*2)));
    imgSrc->zoomIn();
}

void updateCopy(Image *imgSrc, Image original, sf::RenderWindow *window, int *beginX)
{
    *beginX = original.getSize().x*2;
    window->setSize(sf::Vector2u(*beginX + SIZEBUTTONX, original.getSize().y));
    imgSrc->copy(original);
}

void updateRotateLeft(Image *imgSrc, Image original, sf::RenderWindow *window, int *beginX)
{
    *beginX = original.getSize().x + imgSrc->getSize().y;
    window->setSize(sf::Vector2u(*beginX + SIZEBUTTONX, std::max(original.getSize().y, imgSrc->getSize().x)));
    imgSrc->rotateLeft();
}

void updateRotateRight(Image *imgSrc, Image original, sf::RenderWindow *window, int *beginX)
{
    *beginX = original.getSize().x + imgSrc->getSize().y;
    window->setSize(sf::Vector2u(*beginX + SIZEBUTTONX, std::max(original.getSize().y, imgSrc->getSize().x)));
    imgSrc->rotateRight();
}

void updateConvolution(Image *img, std::vector<tgui::EditBox::Ptr> *kernelText)
{
    bool isEmpty = false;
    std::vector<std::vector<float> > kernel;
    unsigned int k = 0;
    kernel.resize(KERNELSIZE);

    for(unsigned int i = 0; i < KERNELSIZE; i++)
    {
        kernel[i].resize(KERNELSIZE);
        for(unsigned int j = 0; j < KERNELSIZE; j++)
        {
            kernel[i][j] = std::stof((*kernelText)[k]->getText();
            k++;
            std::cout << kernel[i][j] << ' ';
        }
        std::cout << '\n';
    }

    if(!isEmpty)
    {
        img->convolution(kernel);
    }
}

void initializeKernel(std::vector<tgui::EditBox::Ptr> *kernelComponents, tgui::Gui *gui, int *beginX, sf::Vector2f size)
{
    tgui::EditBox::Ptr auxiliarEBOX;
    for(unsigned int i = 0; i < KERNELSIZE; i++)
    {
        for(unsigned int j = 0; j < KERNELSIZE; j++)
        {
            auxiliarEBOX = tgui::EditBox::create();
            auxiliarEBOX->setInputValidator(tgui::EditBox::Validator::Float);
            auxiliarEBOX->setAlignment(tgui::EditBox::Alignment::Center);
            auxiliarEBOX->setSize(float(size.x)/KERNELSIZE, size.y);
            auxiliarEBOX->setPosition(*beginX + j*float(size.x)/KERNELSIZE, (i+CONVOLUTIONCOMPONENTS)*size.y);
            gui->add(auxiliarEBOX);
            kernelComponents.push_back(auxiliarEBOX);
        }
    }
}

void createWidgets(tgui::Gui *gui, sf::RenderWindow *window, Image *img, Image original, int *beginX, sf::Vector2f size, int *scales, int *bias, float *alpha)
{
    std::vector<tgui::Widget::Ptr> widgets;

    // Copy Button
    tgui::Button::Ptr copyButton = tgui::Button::create("Copy");
    copyButton->connect("pressed", &updateCopy, img, original, window, beginX);
    widgets.push_back(copyButton);

    // Save Button
    tgui::Button::Ptr saveButton = tgui::Button::create("Save");
    saveButton->connect("pressed", &Image::saveToFile, img, "output.jpg");
    widgets.push_back(saveButton);

    // Flip Vertical
    tgui::Button::Ptr flipVerticalButton = tgui::Button::create("Flip Vertical");
    flipVerticalButton->connect("pressed", &Image::flipVertical, img);
    widgets.push_back(flipVerticalButton);

     // Flip Horizontal
    tgui::Button::Ptr flipHorizontalButton = tgui::Button::create("Flip Horizontal");
    flipHorizontalButton->connect("pressed", &Image::flipHorizontal, img);
    widgets.push_back(flipHorizontalButton);

    // Grey Scale
    tgui::Button::Ptr greyScaleButton = tgui::Button::create("Grey Scale");
    greyScaleButton->connect("pressed", &Image::toGreyScale, img);
    widgets.push_back(greyScaleButton);

    // Quantize Button
    widgets.push_back(tgui::TextBox::create());

    // Histogram
    tgui::Button::Ptr histogramButton = tgui::Button::create("Show Histogram");
    histogramButton->connect("pressed", &showHistogram, img, "Histogram");
    widgets.push_back(histogramButton);

    // Brightness Button
    widgets.push_back(tgui::TextBox::create());

    // Contrast Button
    tgui::EditBox::Ptr contrastText = tgui::EditBox::create();
    contrastText->setInputValidator(tgui::EditBox::Validator::Float);
    contrastText->setDefaultText(sf::String("1"));
    contrastText->setText(sf::String("1"));
    contrastText->setAlignment(tgui::EditBox::Alignment::Center);
    widgets.push_back(contrastText);
    tgui::Button::Ptr contrastButton = tgui::Button::create("Adjust Contrast");
    contrastButton->connect("pressed", &updateContrast, img, contrastText, alpha);
    widgets.push_back(contrastButton);

    // Negative Button
    tgui::Button::Ptr negativeButton = tgui::Button::create("Negative");
    negativeButton->connect("pressed", &Image::toNegative, img);
    widgets.push_back(negativeButton);

    // Equalization Button
    tgui::Button::Ptr equalizationButton = tgui::Button::create("Equalize Histogram");
    equalizationButton->connect("pressed", &equalizeAndPlot, img);
    widgets.push_back(equalizationButton);

    // Histogram Matching Button
    tgui::EditBox::Ptr matchingText = tgui::EditBox::create();
    widgets.push_back(matchingText);
    tgui::Button::Ptr matchingButton = tgui::Button::create("Match Histograms");
    matchingButton->connect("pressed", &updateMatching, img, matchingText);
    widgets.push_back(matchingButton);

    // Zoom out
    tgui::Button::Ptr zoomOutButton = tgui::Button::create("Zoom Out");
    tgui::EditBox::Ptr sxText = tgui::EditBox::create();
    sxText->setInputValidator(tgui::EditBox::Validator::UInt);
    sxText->setDefaultText(sf::String("Width scale"));
    sxText->setSize(float(size.x)/2, size.y);
    sxText->setPosition(*beginX, ZOOMOUTTEXT*size.y);
    gui->add(sxText);
    tgui::EditBox::Ptr syText = tgui::EditBox::create();
    syText->setInputValidator(tgui::EditBox::Validator::UInt);
    syText->setDefaultText(sf::String("Height scale"));
    syText->setSize(float(size.x)/2, size.y);
    syText->setPosition((*beginX + float(size.x)/2), ZOOMOUTTEXT*size.y);
    gui->add(syText);
    zoomOutButton->connect("pressed", &updateZoomOut, img, sxText, syText, original, window, beginX);
    widgets.push_back(sxText);
    widgets.push_back(zoomOutButton);

    // Zoom in
    tgui::Button::Ptr zoomInButton = tgui::Button::create("Zoom in");
    zoomInButton->connect("pressed", &updateZoomIn, img, original, window, beginX);
    widgets.push_back(zoomInButton);

    // Rotate Left
    tgui::Button::Ptr rotateLeftButton = tgui::Button::create("Rotate Left");
    rotateLeftButton->setSize(float(size.x)/2, size.y);
    rotateLeftButton->setPosition(*beginX, ROTATEBUTTON*size.y);
    rotateLeftButton->connect("pressed", &updateRotateLeft, img, original, window, beginX);
    gui->add(rotateLeftButton);
    widgets.push_back(rotateLeftButton);

    // Rotate Left
    tgui::Button::Ptr rotateRightButton = tgui::Button::create("Rotate Right");
    rotateRightButton->setSize(float(size.x)/2, size.y);
    rotateRightButton->setPosition(*beginX + float(size.x)/2, ROTATEBUTTON*size.y);
    rotateRightButton->connect("pressed", &updateRotateRight, img, original, window, beginX);
    gui->add(rotateRightButton);

    // Convolution
    tgui::Button::Ptr convolutionButton = tgui::Button::create("Convolution");
    std::vector<tgui::EditBox::Ptr> kernelComponents;
    initializeKernel(&kernelComponents, gui, beginX, size);
    convolutionButton->connect("pressed", &updateConvolution, img, &kernelComponents);
    widgets.push_back(convolutionButton);
    widgets.push_back(tgui::TextBox::create());
    widgets.push_back(tgui::TextBox::create());
    widgets.push_back(tgui::TextBox::create());

    for(unsigned int i = 0; i < NWIDGETS; i++)
    {
        if(i != QUANTIZEBUTTON && i != BRIGHTNESSBUTTON && i != ZOOMOUTTEXT && i != ROTATEBUTTON && i < CONVOLUTIONCOMPONENTS)
        {
            widgets[i]->setPosition(*beginX, i*size.y);
            widgets[i]->setSize(size.x, size.y);
            gui->add(widgets[i]);
        }

    }
}

int main(int argc, char *argv[])
{
    Image original, changed;
    original.loadFromFile(argv[1]);
    changed.loadFromFile(argv[1]);
    changed.setPosition(original.getSize().x, 0);
    sf::Event event;
    sf::RenderWindow window(sf::VideoMode(original.getSize().x*2 + SIZEBUTTONX, original.getSize().y), "OP Toolkit");

    int beginX = original.getSize().x*2;
    int scales = SCALESNUMBER;
    int bias = 0;
    float alpha = 1;
    std::string srcMatching;

    /// GUI's stuff
    tgui::Gui gui{window};

    /// Widget's stuff

    // Quantize Button
    SliderButton quantizeButton;
    quantizeButton.initialize(&gui, sf::Vector2f(1, SCALESNUMBER), "Quantize", Image::quantize, &changed, &scales);
    quantizeButton.setSize(sf::Vector2f(SIZEBUTTONX, float(original.getSize().y)/NWIDGETS), float(3)/4);
    quantizeButton.setPosition(sf::Vector2f(original.getSize().x*2, QUANTIZEBUTTON*(float(original.getSize().y)/NWIDGETS)));

    // Brightness Button
    SliderButton brightnessButton;
    brightnessButton.initialize(&gui, sf::Vector2f(-(SCALESNUMBER-1), (SCALESNUMBER-1)), "Adjust Brightness", Image::adjustBrightness, &changed, &bias);
    brightnessButton.setSize(sf::Vector2f(SIZEBUTTONX, float(original.getSize().y)/NWIDGETS),float(3)/4);
    brightnessButton.setPosition(sf::Vector2f(original.getSize().x*2, BRIGHTNESSBUTTON*(float(original.getSize().y)/NWIDGETS)));

    // Convolution Button

    createWidgets(&gui, &window, &changed, original, &beginX, sf::Vector2f(float(SIZEBUTTONX), float(original.getSize().y)/float(NWIDGETS)), &scales, &bias, &alpha);

    /// Window's loop
    while (window.isOpen())
    {

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }

            gui.handleEvent(event);
        }

        window.clear(sf::Color(255,255,255));
        window.draw(original.getSprite());
        window.draw(changed.getSprite());
        gui.draw();
        window.display();
    }

    return 0;
}
