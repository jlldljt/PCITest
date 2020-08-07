
Public Class Form1
    'Public Const GENERIC_WRITE = &H40000000
    'Public Const GENERIC_READ = &H80000000
    'Const FILE_ATTRIBUTE_NORMAL = &H80
    'Const CREATE_ALWAYS = 2
    'Const OPEN_EXISTING = 3
    'Const OPEN_ALWAYS = 4
    'Const INVALID_HANDLE_VALUE = -1
    'Const FILE_NAME = "COM4"     'This can be any file that does not
    ''currently exist.

    'Structure MyType
    '    Dim value As Integer
    'End Structure


    'Declare Function ReadFile Lib "kernel32" (ByVal hFile As Long, _
    '   ByRef lpBuffer As Object, ByVal nNumberOfBytesToRead As Long, _
    '   lpNumberOfBytesRead As Long, ByVal lpOverlapped As Long) As Long

    'Private Declare Function CloseHandle Lib "kernel32" ( _
    '  ByVal hObject As Long) As Long

    'Private Declare Function WriteFile Lib "kernel32" ( _
    '  ByVal hFile As Long, lpBuffer As Any, _
    '  ByVal nNumberOfBytesToWrite As Long, _
    '  lpNumberOfBytesWritten As Long, ByVal lpOverlapped As Long) As Long

    'Private Declare Function CreateFile Lib "kernel32" _
    '  Alias "CreateFileA" (ByVal lpFileName As String, _
    '  ByVal dwDesiredAccess As Long, ByVal dwShareMode As Long, _
    '  ByVal lpSecurityAttributes As Long, _
    '  ByVal dwCreationDisposition As Long, _
    '  ByVal dwFlagsAndAttributes As Long, ByVal hTemplateFile As Long) _
    '  As Long

    'Declare Function FlushFileBuffers Lib "kernel32" ( _
    '  ByVal hFile As Long) As Long

    Dim Addressreg_crc As Integer
    Dim HiByte As Integer
    Dim LoByte As Double
    Dim k As Integer
    Dim com As Long
    Dim RcvStr(100) As Byte

    Function crc16(ByRef cmdstring() As Byte, ByVal j As Integer)
        Dim data As Integer
        Dim i As Integer


        Addressreg_crc = &HFFFF
        For i = 0 To j
            Addressreg_crc = Addressreg_crc Xor cmdstring(i)
            For j = 0 To 7
                data = Addressreg_crc And &H1
                If data Then
                    Addressreg_crc = Int(Addressreg_crc / 2)
                    Addressreg_crc = Addressreg_crc And &H7FFF
                    Addressreg_crc = Addressreg_crc Xor &HA001
                Else
                    Addressreg_crc = Addressreg_crc / 2
                    Addressreg_crc = Addressreg_crc And &H7FFF
                End If
            Next j
        Next i

        If Addressreg_crc < 0 Then
            Addressreg_crc = Addressreg_crc - &HFFFF0000
        End If

        HiByte = Addressreg_crc And &HFF
        LoByte = (Addressreg_crc And &HFF00) / &H100
        Return Addressreg_crc
    End Function

    Private Sub Button1_Click(sender As System.Object, e As System.EventArgs) Handles Button1.Click
        Dim SendStr(8) As Byte
        Dim addr As UShort
        Dim value As UShort
        addr = Val(TextBox1.Text)
        'Dim RcvStr() As Byte
        Dim CmdLenth As Integer
        SendStr(0) = 1 '从站号是1
        SendStr(1) = &H3 '读多个字的命令代码
        SendStr(2) = (addr - addr Mod 256) / 256 And &HFF '起始地址高字节
        SendStr(3) = addr And &HFF '起始地址低字节
        SendStr(4) = &H0 '数据长度高字节
        SendStr(5) = 1 '数据长度低字节
        Call crc16(SendStr, 5) 'CRC 计算
        SendStr(6) = HiByte
        SendStr(7) = LoByte
        '读命令发送后,当接收 5 + SendStr(5) * 2 个字节时产生中断
        CmdLenth = 5 + SendStr(5) * 2

        Call send(SendStr, 8)

        Threading.Thread.Sleep(1000)
        Dim recvlen As UShort
        recvlen = recv(RcvStr, CmdLenth)

        If recvlen > 0 Then
            value = RcvStr(3) * 256 + RcvStr(4)
            TextBox2.Text = Str(value)
            MessageBox.Show("成功")
        Else
            MessageBox.Show("失败")
        End If

    End Sub

    Private Sub Button2_Click(sender As System.Object, e As System.EventArgs) Handles Button2.Click
        Dim WriteStr() As Byte
        Dim addr As UShort
        Dim value As UShort
        addr = Val(TextBox1.Text)
        value = Val(TextBox2.Text)
        k = 2 '写6 个字节
        ReDim WriteStr(8 + k)
        WriteStr(0) = 1 '从站号是1
        WriteStr(1) = &H10 '写多个字的命令代码
        WriteStr(2) = (addr - addr Mod 256) / 256 And &HFF '起始地址高字节
        WriteStr(3) = addr And &HFF '起始地址低字节
        WriteStr(4) = &H0 '数据长度高字节<字的个数>
        WriteStr(5) = k / 2 '数据长度低字节<字的个数>
        WriteStr(6) = k '数据长度<字节的个数>
        Dim mid As UShort
        'mid = (value - value Mod 256) / 256 '整型除出小数再四舍五入，不是我们想看到的
        WriteStr(7) = (value - value Mod 256) / 256 And &HFF '写的第1 个字的高字节
        WriteStr(8) = value And &HFF '写的第1 个字的低字节
        'WriteStr(7) = &H12 '写的第1 个字的高字节
        'WriteStr(8) = &H34 '写的第1 个字的低字节
        Call crc16(WriteStr, 6 + k)
        WriteStr(9) = HiByte
        WriteStr(10) = LoByte

        Call send(WriteStr, 11)
        Threading.Thread.Sleep(1000)
        Dim recvlen As UShort
        recvlen = recv(RcvStr, 8)

        If recvlen > 0 Then
            MessageBox.Show("成功")
        Else
            MessageBox.Show("失败")
        End If

    End Sub

    Private Sub Form1_Load(sender As System.Object, e As System.EventArgs) Handles MyBase.Load
        'com = CreateFile(FILE_NAME, GENERIC_WRITE Or GENERIC_READ, _
        '                     0, 0, OPEN_EXISTING, 0, 0)

        GetSerialPortNames()
        GetSerialBaudRate()
    End Sub

    Function recv(buf() As Byte, ByVal num As Integer)
        Dim recvlen As UShort
        recvlen = SerialPort1.Read(buf, 0, num)
        Return recvlen

        'For i As Int16 = 0 To bytesRead - 1
        '    indata = indata & DecToHex(ch(i))
        'Next
    End Function

    Sub send(buf() As Byte, ByVal num As Integer)
        'serialPort1.WriteLine(str);
        SerialPort1.DiscardOutBuffer()
        SerialPort1.DiscardInBuffer()
        SerialPort1.Write(buf, 0, num)
        'MessageBox()
    End Sub

    Private Sub ComboBox1_SelectedIndexChanged(sender As System.Object, e As System.EventArgs) Handles ComboBox1.SelectedIndexChanged

    End Sub

    Sub GetSerialPortNames()
        For Each sp As String In My.Computer.Ports.SerialPortNames
            ComboBox1.Items.Add(sp)
        Next
        If ComboBox1.Items.Count > 0 Then
            ComboBox1.SelectedIndex = 0
        End If
    End Sub

    Sub GetSerialBaudRate()

        ComboBox2.Items.Add(9600)
        ComboBox2.Items.Add(19200)
        ComboBox2.Items.Add(115200)

        If ComboBox2.Items.Count > 0 Then
            ComboBox2.SelectedIndex = 0
        End If
    End Sub

    Private Sub Button3_Click(sender As System.Object, e As System.EventArgs) Handles Button3.Click
        If SerialPort1.IsOpen Then
            SerialPort1.Close()
        End If
        SerialPort1.PortName = ComboBox1.Text
        SerialPort1.BaudRate = Val(ComboBox2.Text)
        SerialPort1.Parity = IO.Ports.Parity.None '奇偶校验
        SerialPort1.DataBits = 8 '数据位
        SerialPort1.StopBits = IO.Ports.StopBits.One '停止位
        SerialPort1.Open()
        MessageBox.Show("打开" + ComboBox1.Text)
    End Sub
End Class
