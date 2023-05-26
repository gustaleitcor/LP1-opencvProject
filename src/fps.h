void attFPS(double &fps, int &frameCount, double &startTime)
{
    frameCount++;

    double currentTime = (cv::getTickCount() - startTime) / cv::getTickFrequency();

    if (currentTime >= 1.0)
    {
        fps = frameCount / currentTime;
        startTime = cv::getTickCount();
        frameCount = 0;
    }
}