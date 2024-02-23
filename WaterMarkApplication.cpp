#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;


//метод для внедрения водяного знака в виде текста

void injectWatermarkWithContourSync(Mat& inputImage, const Mat& watermark) {
	// Преобразование изображения в оттенки серого
	Mat grayImage;
	cvtColor(inputImage, grayImage, COLOR_BGR2GRAY);

	// Бинаризация изображения
	Mat binaryImage;
	threshold(grayImage, binaryImage, 128, 255, THRESH_BINARY_INV);

	// Поиск контуров на бинаризованном изображении
	std::vector<std::vector<Point>> contours;
	findContours(binaryImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	// Внедрение водяного знака в определенные точки контуров
	for (const auto& contour : contours) {
		for (const auto& point : contour) {
			// Внедрение водяного знака в точку контура
			// Опционально можно применить логику синхронизации

			// Проверка, чтобы точка была внутри границ изображения
			if (point.x >= 0 && point.x < inputImage.cols && point.y >= 0 && point.y < inputImage.rows) {
				// Применение водяного знака
				inputImage.at<Vec3b>(point) = watermark.at<Vec3b>(point);
			}
		}
	}
}


void addWatermark(Mat& image, const string& watermarkText, bool dublicate, Point watermarkPosition, double alpha, Scalar textColor, double textScale, int thickness) {
	Size textSize = getTextSize(watermarkText, FONT_HERSHEY_SIMPLEX, textScale, thickness, nullptr);
	Mat watermark = Mat::zeros(image.rows, image.cols, image.type());

	if (dublicate == true)
	{
		// Дублирование текста по диагонали
		for (int y = 0; y < watermark.rows; y += textSize.height) {
			for (int x = 0; x < watermark.cols; x += textSize.width) {
				putText(watermark, watermarkText, Point(x, y), FONT_HERSHEY_SIMPLEX, textScale, textColor, thickness);
			}
		}
	}
	else
	{
		putText(watermark, watermarkText, watermarkPosition, FONT_HERSHEY_SIMPLEX, textScale, textColor, thickness);
	}
	
	// Наложение водяного знака на изображение
	addWeighted(image, 1 - alpha, watermark, alpha, 0, image);
}

int main() {
	while (true)
	{
		int method;
		cout << "Change method: 1 or 2: ";
		cin >> method;

		switch (method)
		{
			case 1:
			{
				cout << "Enter the image:";
				string imageWay;
				cin >> imageWay;
				Mat image = imread(imageWay);

				if (image.empty()) {
					cout << "Image not found!" << endl;
					return -1;
				}

				string watermarkText;
				int x, y;


				cout << "Enter the text: ";
				cin >> watermarkText;

				cout << "Enter the x: ";
				cin >> x;

				cout << "Enter the y: ";
				cin >> y;

				Point watermarkPosition(x, y);

				int R, G, B;

				cout << "Enter the Red: ";
				cin >> R;

				cout << "Enter the Green: ";
				cin >> G;

				cout << "Enter the Blue: ";
				cin >> B;

				Scalar textColor(R, G, B);

				double alpha;
				cout << "Enter the alpha:";
				cin >> alpha;

				double textScale;
				cout << "Enter the Scale:";
				cin >> textScale;

				double thickness;
				cout << "Enter the thickness:";
				cin >> thickness;

				bool dublicate;
				string temp;
				cout << "Do you want to dublicate?";
				cin >> temp;

				if (temp == "Yes")
					dublicate = true;
				else if (temp == "No")
					dublicate = false;

				addWatermark(image, watermarkText, dublicate, watermarkPosition, alpha, textColor, textScale, thickness);

				imwrite("ImageWithText.jpg", image);
			}
			break;

			case 2:
			{
				cout << "Enter the image:";
				string imageWay;
				cin >> imageWay;
				Mat image = imread(imageWay);

				cout << "Enter the watermark:";
				cin >> imageWay;
				Mat water = imread(imageWay);

				if (image.empty()) {
					cout << "Image not found!" << endl;
					return -1;
				}
				if (water.empty()) {
					cout << "Watermark not found!" << endl;
					return -1;
				}

				resize(water, water, image.size());
				injectWatermarkWithContourSync(image, water);
				imwrite("ImageContour.jpg", image);
			}
			break;
		}
	}
	return 0;
}

