# Generating AWS xclbin from normal xclbin

AWS requires the `*.xclbin` file to be customized for F1 instance. After 
`*.xclbin` file has been generated through the `make` process, you need some
additional steps to generate `*.awsxclbin` file.

More information can be found at the [AWS FPGA Github repository](https://github.com/aws/aws-fpga).

## Download AWS FPGA Github on F1

Log onto your F1 instance, and do:

```
git clone https://github.com/aws/aws-fpga.git
cd aws-fpga
source vitis_runtime_setup.sh
source vitis_setup.sh
```

You need to see no errors to proceed.

## Prepare S3 Bucket

Follow the 
[steps to configure aws-cli and S3 bucket](https://github.com/aws/aws-fpga/blob/master/Vitis/docs/Setup_AWS_CLI_and_S3_Bucket.md). 

Remember you also need to create access point for S3 bucket. This gives you a 
string like:
`arn:aws:s3:us-east-1:186486331965:accesspoint/pynq-helloworld`
Remember to open its access, instead of blocking public traffic.

## Call the AFI Generation Process

We can now use the following command to generate the `*.awsxclbin` file. Adjust
the command based on your own information.

```
./Vitis/tools/create_vitis_afi.sh \
-xclbin=/home/centos/data/build/resizer.xclbin \
-o=resizer \
-s3_bucket=arn:aws:s3:us-east-1:186486331965:accesspoint/pynq-helloworld \
-s3_dcp_key=dcps/ \
-s3_logs_key=logs/
```

The above command is unblocking, while the `*.awsxclbin` file is being 
generated.

Now we can check a file similar to the following to locate the AFI ID:
`20_03_06-205931_afi_id.txt`. 

Use the stored AFI ID to check the generation process:
```
aws ec2 describe-fpga-images --fpga-image-ids <afi-id>
```

Check the state code; it might just show `pending`. 
Only when the state code shows `available` can you 
proceed with the generated awsxclbin. This process can take as long as an
hour so please be patient.
