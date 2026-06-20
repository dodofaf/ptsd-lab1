import json
import boto3
import urllib.parse
import os

s3 = boto3.client('s3')
sqs = boto3.client('sqs')

def lambda_handler(event, context):
    finish_bucket = os.environ['FINISH_BUCKET']
    queue_url = os.environ['SQS_QUEUE_URL']
    
    for record in event['Records']:
        source_bucket = record['s3']['bucket']['name']
        key = urllib.parse.unquote_plus(record['s3']['object']['key'], encoding='utf-8')
        
        copy_source = {'Bucket': source_bucket, 'Key': key}
        
        try:
            # Copy the file to the destination bucket
            s3.copy_object(CopySource=copy_source, Bucket=finish_bucket, Key=key)
            print(f"Successfully copied {key} to {finish_bucket}")
            
            # 5e: Integrate another service (SQS)
            message = {
                "status": "success",
                "message": f"File {key} processed and copied.",
                "destination_bucket": finish_bucket
            }
            sqs.send_message(
                QueueUrl=queue_url,
                MessageBody=json.dumps(message)
            )
            
        except Exception as e:
            print(f"Error copying object {key} from bucket {source_bucket}. Event: {event}")
            print(e)
            raise e
