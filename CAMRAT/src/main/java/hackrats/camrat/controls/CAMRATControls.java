package hackrats.camrat.controls;

import org.opencv.core.Core;

import nu.pattern.OpenCV;

public class CAMRATControls {
	
	static {
		// Make sure that OpenCV is loaded before we do anything else
		OpenCV.loadLocally();
	}
	
	public static void main(String[] args) {
		System.out.printf("Using OpenCV version %s, starting CAMRAT controls...\n", Core.VERSION);
	}
}