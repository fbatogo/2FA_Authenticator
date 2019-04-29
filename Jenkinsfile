pipeline {
  options {
    timestamps()
    buildDiscarder(logRotator(numToKeepStr: '5', artifactNumToKeepStr: '5'))
  }

  agent any
  
  stages {
    stage('Checkout Code') {
      steps {
        git 'https://github.com/fbatogo/Rollin.git'

	// Get the submodules as well.
	sh '''git submodule update --init'''
      }
    }
    stage('Build and Run Tests') {
      steps {
	  sh '''qmake -qt=qt5 QT+=testlib'''
          sh '''make -j4 clean'''
          sh '''/usr/local/sonar/wrapper/build-wrapper-linux-x86-64 --out-dir bw_output make -j4 all'''
      
//	  sh '''make -j2'''
	  sh '''./Rollin --xml'''
          sh '''gcov base32coder.cpp hexdecoder.cpp hmac.cpp hotp.cpp sha1hash.cpp sha1impl.c sha256hash.cpp sha2.c sha512hash.cpp totp.cpp keyentry.cpp keystorage.cpp databasekeystorage.cpp secretdatabase.cpp otphandler.cpp bytearray.cpp'''    // Run coverage output.
      }
    }
    stage('SonarQube analysis') {
        
      steps {
            
        script {
          
          // requires SonarQube Scanner 2.8+
          
          def scannerHome = tool 'SonarQube Scanner';
          
          withSonarQubeEnv('SonarQube in the Cloud') {
            
            sh "${scannerHome}/bin/sonar-scanner"
          
          }
            
        }
        
      }
    
    }    
  }
  post {
    always {
      xunit ( tools: [ QtTest(pattern: "*.xml") ])
    }
  }
}