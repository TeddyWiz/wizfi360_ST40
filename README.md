# wizfi360 and STM32F401CB

테스트 한 보드는 Uart1(serial), Uart2(RS485), Uart6(Wiz360)가 연결되어있으며,
해당 코드는 STM32F401CB 를 이용하여 WizFi360과의 정상적으로 통신이 되는지 확인하기 위한 코드입니다.


# STM32F401 Pinmap

![image](https://user-images.githubusercontent.com/9648281/93292819-67f5e980-f821-11ea-8343-6a92652210fb.png)

# UART1 와 UART6 연결도
UART쪽은 아래와 같이 동작하게 만들었습니다.

![image](https://user-images.githubusercontent.com/9648281/93292678-07ff4300-f821-11ea-8e3f-a377d3a9ea42.png)

UART6 Recv쪽 인터럽터에 RingBuffer를 사용하였으며, 개행문자(\r\n)일 경우 에만 UART1(Serial)에 찍히도록 했습니다.



## UART6 TX로 전송되는 AT Command
![image](https://user-images.githubusercontent.com/9648281/93293061-0b46fe80-f822-11ea-8570-ef8089131017.png)
## UART1 RX로 들어온 결과값
![image](https://user-images.githubusercontent.com/9648281/93292380-68da4b80-f820-11ea-8f46-12c8ea522c93.png)
