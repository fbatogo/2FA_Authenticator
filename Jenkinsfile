pipeline {
  agent any
  stages {
    stage('Checkout Code') {
      steps {
        git 'https://github.com/fbatogo/Rollin.git'
      }
    }
    stage('Build and Run Tests') {
      steps {
        dir("test") {
	  sh '''qmake -qt=qt5 QT+=testlib'''
	  sh '''make -j2'''
	  sh '''./Rollin'''
	}
      }
    }
  }
}